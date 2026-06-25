namespace LoopNext.Metrology;

// Комплексное число одинарной точности, воспроизводящее поведение std::complex<float>
// из эталонного проекта LOOP_NEW. Используется в расчётах фаз кольца и среды, где
// исходный код выполнял всю арифметику во float; применение double давало бы расхождение.
internal readonly struct ComplexF
{
    public readonly float Re;
    public readonly float Im;

    public ComplexF(float re, float im)
    {
        Re = re;
        Im = im;
    }

    public static readonly ComplexF ImaginaryOne = new(0f, 1f);

    public static ComplexF operator +(ComplexF a, ComplexF b) => new(a.Re + b.Re, a.Im + b.Im);

    public static ComplexF operator +(ComplexF a, float b) => new(a.Re + b, a.Im);

    public static ComplexF operator -(float a, ComplexF b) => new(a - b.Re, -b.Im);

    public static ComplexF operator *(ComplexF a, ComplexF b) =>
        new(a.Re * b.Re - a.Im * b.Im, a.Re * b.Im + a.Im * b.Re);

    public static ComplexF operator *(ComplexF a, float b) => new(a.Re * b, a.Im * b);

    public static ComplexF operator /(ComplexF a, ComplexF b)
    {
        var denominator = b.Re * b.Re + b.Im * b.Im;
        return new(
            (a.Re * b.Re + a.Im * b.Im) / denominator,
            (a.Im * b.Re - a.Re * b.Im) / denominator);
    }

    // Аргумент (фаза) комплексного числа. Соответствует std::arg.
    public float Arg() => MathF.Atan2(Im, Re);

    // Комплексная экспонента. Соответствует std::exp(complex<float>).
    public static ComplexF Exp(ComplexF z)
    {
        var magnitude = MathF.Exp(z.Re);
        return new(magnitude * MathF.Cos(z.Im), magnitude * MathF.Sin(z.Im));
    }

    // Главное значение квадратного корня. Соответствует std::sqrt(complex<float>).
    public static ComplexF Sqrt(ComplexF z)
    {
        var modulus = MathF.Sqrt(z.Re * z.Re + z.Im * z.Im);
        var re = MathF.Sqrt((modulus + z.Re) / 2f);
        var im = MathF.Sqrt((modulus - z.Re) / 2f);
        if (z.Im < 0f)
        {
            im = -im;
        }

        return new(re, im);
    }
}
