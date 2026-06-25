using System.IO.Ports;

namespace LoopNext.Hardware;

// Перечисление доступных COM-портов.
// Перенос comboBox_*_com_DropDown (WorkForm.h:7809-7827): список берётся из SerialPort.GetPortNames().
public static class SerialPortEnumerator
{
    public static string[] GetAvailablePortNames() => SerialPort.GetPortNames();
}
