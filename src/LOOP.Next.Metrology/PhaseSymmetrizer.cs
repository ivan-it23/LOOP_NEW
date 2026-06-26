using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Симметризация фаз по матрицам из эталонного проекта (function.cpp:500-720).
// Перенос build_symmetry_matrix*, extract_condition_byte, get_phase_smt, get_air_smt.
// Входные фазы готовятся через apply_rx_phase_orientation с нормализацией к Air_zz,
// затем умножаются на матрицу K, выбранную по (N_Tx, condition).
public static class PhaseSymmetrizer
{
    public const int SmtMaxTx = 5;
    public const int FrequencyCount = 2;

    // Извлекает 8-битную маску работающих передатчиков для частоты из condition.
    // Layout (LSB-first): byte0 = 2000 кГц (freq 1), byte1 = 400 кГц (freq 0).
    // Внутри байта: bit0 = T1, bit1 = T2, ..., bit4 = T5.
    public static byte ExtractConditionByte(uint rawCondition, int frequencyIndex)
    {
        return frequencyIndex == 0
            ? (byte)((rawCondition >> 8) & 0xFFu)
            : (byte)(rawCondition & 0xFFu);
    }

    // Вычисляет симметризованные фазы из GP_TOOL_DATA и метрологии.
    public static V10 GetPhaseSmt(in GpToolData data, in GpToolMetrology metrology)
    {
        var phaseSmt = default(V10);
        phaseSmt.Clear();

        var identity = ToolIdentity.Decode(data.Signature);
        var nTx = identity.TransmitterCount;
        var rxPosition = metrology.RxPosition == 1 ? 1u : 0u;

        for (var frequency = 0; frequency < FrequencyCount; frequency++)
        {
            var condition = ExtractConditionByte(data.Condition, frequency);
            var matrix = BuildSymmetryMatrix(nTx, condition);

            // Подготовка входных разностей фаз: нормализация к воздуху и ориентация.
            var deltas = new float[SmtMaxTx];
            for (var tx = 0; tx < nTx && tx < SmtMaxTx; tx++)
            {
                var rawDelta = data.GetDeltaPhase(frequency, tx);
                var airZz = metrology.GetAirZz(frequency, tx) / (float)PhysicalConstants.Milligrad;
                deltas[tx] = PhaseMath.ApplyRxPhaseOrientation(rawDelta, airZz, (uint)tx, rxPosition);
            }

            // phase_smt[freq * N_Tx + t] = sum_j K[t][j] * deltas[j].
            for (var t = 0; t < nTx && t < SmtMaxTx; t++)
            {
                var sum = 0.0f;
                for (var j = 0; j < SmtMaxTx; j++)
                {
                    sum += matrix[t, j] * deltas[j];
                }

                var idx = frequency * (int)nTx + t;
                if (idx < V10.ChannelCount)
                {
                    phaseSmt.SetValue(idx, sum);
                }
            }
        }

        return phaseSmt;
    }

    // Симметризует уже подготовленный V10 (например, локальный воздух) с полной маской Tx.
    public static V10 GetAirSmt(in V10 input, in GpToolMetrology metrology)
    {
        var phaseSmt = default(V10);
        phaseSmt.Clear();

        var identity = ToolIdentity.Decode(metrology.Signature);
        var nTx = identity.TransmitterCount;
        var condition = (byte)((1u << (int)nTx) - 1u);
        var matrix = BuildSymmetryMatrix(nTx, condition);

        for (var frequency = 0; frequency < FrequencyCount; frequency++)
        {
            for (var t = 0; t < nTx && t < SmtMaxTx; t++)
            {
                var sum = 0.0f;
                for (var j = 0; j < SmtMaxTx; j++)
                {
                    var inIdx = frequency * (int)nTx + j;
                    if (j < nTx && inIdx < V10.ChannelCount)
                    {
                        sum += matrix[t, j] * input.GetValue(inIdx);
                    }
                }

                var idx = frequency * (int)nTx + t;
                if (idx < V10.ChannelCount)
                {
                    phaseSmt.SetValue(idx, sum);
                }
            }
        }

        return phaseSmt;
    }

    // Диспетчер выбора матрицы по (N_Tx, condition).
    public static float[,] BuildSymmetryMatrix(uint nTx, byte condition)
    {
        var matrix = new float[SmtMaxTx, SmtMaxTx];
        ZeroSmtMatrix(matrix);

        var resolved = false;
        if (nTx == 5)
            resolved = TryBuildSymmetryMatrix5Tx(matrix, condition);
        else if (nTx == 4)
            resolved = TryBuildSymmetryMatrix4Tx(matrix, condition);
        else if (nTx == 3)
            resolved = TryBuildSymmetryMatrix3Tx(matrix, condition);

        if (!resolved)
            FillIdentitySymmetryMatrix(matrix, condition, (int)nTx);

        return matrix;
    }

    private static void ZeroSmtMatrix(float[,] matrix)
    {
        for (var row = 0; row < SmtMaxTx; row++)
        {
            for (var col = 0; col < SmtMaxTx; col++)
            {
                matrix[row, col] = 0.0f;
            }
        }
    }

    private static void CopySmtMatrix(float[,] destination, float[,] source)
    {
        for (var row = 0; row < SmtMaxTx; row++)
        {
            for (var col = 0; col < SmtMaxTx; col++)
            {
                destination[row, col] = source[row, col];
            }
        }
    }

    private static void FillIdentitySymmetryMatrix(float[,] matrix, byte condition, int validTxCount)
    {
        ZeroSmtMatrix(matrix);
        for (var t = 0; t < validTxCount && t < SmtMaxTx; t++)
        {
            var working = ((condition >> t) & 0x1u) != 0;
            matrix[t, t] = working ? 1.0f : 0.0f;
        }
    }

    private static bool TryBuildSymmetryMatrix5Tx(float[,] matrix, byte condition)
    {
        switch (condition)
        {
            case 0x1F:
            case 0xFF:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
                    { +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
                    { +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
                    { +0.00f, +0.00f, +0.25f, +0.50f, +0.25f },
                    { +0.00f, +0.00f, -0.25f, +0.50f, +0.75f }
                });
                return true;
            case 0x1E:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.00f, +1.25f, +0.50f, -0.75f, +0.00f },
                    { +0.00f, +0.75f, +0.50f, -0.25f, +0.00f },
                    { +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
                    { +0.00f, +0.00f, +0.25f, +0.50f, +0.25f },
                    { +0.00f, +0.00f, -0.25f, +0.50f, +0.75f }
                });
                return true;
            case 0x1D:
                CopySmtMatrix(matrix, new[,]
                {
                    { +1.25f, +0.00f, -0.75f, +0.50f, +0.00f },
                    { +0.75f, +0.00f, -0.25f, +0.50f, +0.00f },
                    { +0.00f, +0.00f, +0.75f, +0.50f, -0.25f },
                    { +0.00f, +0.00f, +0.25f, +0.50f, +0.25f },
                    { +0.00f, +0.00f, -0.25f, +0.50f, +0.75f }
                });
                return true;
            case 0x1B:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.50f, +0.75f, +0.00f, -0.25f, +0.00f },
                    { +0.00f, +1.25f, +0.00f, -0.75f, +0.50f },
                    { +0.00f, +0.75f, +0.00f, -0.25f, +0.50f },
                    { +0.00f, +0.25f, +0.00f, +0.25f, +0.50f },
                    { +0.00f, -0.25f, +0.00f, +0.75f, +0.50f }
                });
                return true;
            case 0x17:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
                    { +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
                    { -0.25f, +0.50f, +0.75f, +0.00f, +0.00f },
                    { +0.00f, +0.50f, -0.25f, +0.00f, +0.75f },
                    { +0.00f, +0.50f, -0.75f, +0.00f, +1.25f }
                });
                return true;
            case 0x0F:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
                    { +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
                    { +0.50f, +0.25f, +0.50f, +0.25f, +0.00f },
                    { +0.00f, -0.25f, +0.50f, +0.75f, +0.00f },
                    { +0.00f, -0.75f, +0.50f, +1.25f, +0.00f }
                });
                return true;
            case 0x00:
                FillIdentitySymmetryMatrix(matrix, 0x1F, 5);
                return true;
            default:
                return false;
        }
    }

    private static bool TryBuildSymmetryMatrix4Tx(float[,] matrix, byte condition)
    {
        switch (condition)
        {
            case 0x0F:
            case 0xFF:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
                    { +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
                    { +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
                    { +0.00f, -0.25f, +0.50f, +0.75f, +0.00f },
                    { +0.00f, +0.00f, +0.00f, +0.00f, +0.00f }
                });
                return true;
            case 0x0E:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.00f, +1.25f, +0.50f, -0.75f, +0.00f },
                    { +0.00f, +0.75f, +0.50f, -0.25f, +0.00f },
                    { +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
                    { +0.00f, -0.25f, +0.50f, +0.75f, +0.00f },
                    { +0.00f, +0.00f, +0.00f, +0.00f, +0.00f }
                });
                return true;
            case 0x0D:
                CopySmtMatrix(matrix, new[,]
                {
                    { +1.25f, +0.00f, -0.75f, +0.50f, +0.00f },
                    { +0.75f, +0.00f, -0.25f, +0.50f, +0.00f },
                    { +0.25f, +0.00f, +0.25f, +0.50f, +0.00f },
                    { -0.25f, +0.00f, +0.75f, +0.50f, +0.00f },
                    { +0.00f, +0.00f, +0.00f, +0.00f, +0.00f }
                });
                return true;
            case 0x0B:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.50f, +0.75f, +0.00f, -0.25f, +0.00f },
                    { +0.50f, +0.25f, +0.00f, +0.25f, +0.00f },
                    { +0.50f, -0.25f, +0.00f, +0.75f, +0.00f },
                    { -0.50f, -0.75f, +0.00f, +1.25f, +0.00f },
                    { +0.00f, +0.00f, +0.00f, +0.00f, +0.00f }
                });
                return true;
            case 0x07:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
                    { +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
                    { -0.25f, +0.50f, +0.75f, +0.00f, +0.00f },
                    { -0.75f, +0.50f, +1.25f, +0.00f, +0.00f },
                    { +0.00f, +0.00f, +0.00f, +0.00f, +0.00f }
                });
                return true;
            case 0x00:
                FillIdentitySymmetryMatrix(matrix, 0x0F, 4);
                return true;
            default:
                return false;
        }
    }

    private static bool TryBuildSymmetryMatrix3Tx(float[,] matrix, byte condition)
    {
        switch (condition)
        {
            case 0x07:
                CopySmtMatrix(matrix, new[,]
                {
                    { +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
                    { +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
                    { -0.25f, +0.50f, +0.75f, +0.00f, +0.00f },
                    { -0.75f, +0.50f, +1.25f, +0.00f, +0.00f },
                    { +0.00f, +0.00f, +0.00f, +0.00f, +0.00f }
                });
                return true;
            case 0x00:
                FillIdentitySymmetryMatrix(matrix, 0x07, 3);
                return true;
            default:
                return false;
        }
    }
}
