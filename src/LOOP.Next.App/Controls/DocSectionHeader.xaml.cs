using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace LOOP_Next_App.Controls;

// Заголовок раздела вкладки "Документация": номер и название.
// Оформление статическое, содержимое задаётся разметкой руководства.
public sealed partial class DocSectionHeader : UserControl
{
    public static readonly DependencyProperty NumberProperty =
        DependencyProperty.Register(nameof(Number), typeof(string), typeof(DocSectionHeader),
            new PropertyMetadata(string.Empty, OnNumberChanged));

    public static readonly DependencyProperty TitleProperty =
        DependencyProperty.Register(nameof(Title), typeof(string), typeof(DocSectionHeader),
            new PropertyMetadata(string.Empty, OnTitleChanged));

    public DocSectionHeader()
    {
        InitializeComponent();
    }

    public string Number
    {
        get => (string)GetValue(NumberProperty);
        set => SetValue(NumberProperty, value);
    }

    public string Title
    {
        get => (string)GetValue(TitleProperty);
        set => SetValue(TitleProperty, value);
    }

    private static void OnNumberChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        if (d is DocSectionHeader header)
        {
            header.NumberText.Text = e.NewValue as string ?? string.Empty;
        }
    }

    private static void OnTitleChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        if (d is DocSectionHeader header)
        {
            header.TitleText.Text = e.NewValue as string ?? string.Empty;
        }
    }
}
