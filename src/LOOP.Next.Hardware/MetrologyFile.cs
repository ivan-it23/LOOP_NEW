using System.Runtime.InteropServices;
using LoopNext.Core.Models;

namespace LoopNext.Hardware;

public static class MetrologyFile
{
    public static int BinarySize => Marshal.SizeOf<GpToolMetrology>();

    public static GpToolMetrology Read(string path)
    {
        var bytes = File.ReadAllBytes(path);
        if (bytes.Length < BinarySize)
        {
            throw new InvalidDataException($"Файл метрологии короче {BinarySize} байт.");
        }

        return MemoryMarshal.Read<GpToolMetrology>(bytes.AsSpan(0, BinarySize));
    }

    public static void Write(string path, in GpToolMetrology metrology)
    {
        var bytes = new byte[BinarySize];
        var value = metrology;
        MemoryMarshal.Write(bytes, in value);
        File.WriteAllBytes(path, bytes);
    }
}
