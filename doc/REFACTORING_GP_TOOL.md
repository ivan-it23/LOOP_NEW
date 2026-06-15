# LOOP_NEW: Рефакторинг на универсальные структуры GP_TOOL_DATA и GP_TOOL_METROLOGY

## Дата
2026-06-03

## Цель
Заменить устаревшие жёстко-типизированные структуры данных (`METROLOGY`, `METROLOGY_NEW`, `AllData_1DDS`, `DATA_5_TX`, `V8`, `DAT`) на универсальные `GP_TOOL_DATA` и `GP_TOOL_METROLOGY`, поддерживающие приборы с 3/4/5 передатчиками через динамическое определение `N_Tx` из сигнатуры.

---

## 1. Удалённые структуры (`variable.h`)

| Структура | Размер | Что было | Причина удаления |
|-----------|--------|----------|-----------------|
| `V8` | 32 байта | 8 float-каналов | Заменён на `V10` (10 каналов) |
| `METROLOGY` | 268 байт | L1[8], L2[8], F[8], Air_zz[10] | Жёсткая привязка к 8 каналам |
| `METROLOGY_NEW` | 240 байт | L1[5], L2[5], F[2], Air_zz[10], Air_zz_amt, inc_set, PhaseOffsets | Универсальная, но лишние поля; заменена на `GP_TOOL_METROLOGY` |
| `METROLOGY_FOR_AUTONOM` | 256 байт | Субмножество METROLOGY | Не использовалось в живом коде |
| `AllData_1DDS` | 232 байта | PH_RX_1[8], PH_RX_2[8], Ro_smt[8] | Жёсткая привязка к 8 каналам |
| `DATA_5_TX` | 240 байт | DELTA_PH_400[5], DELTA_PH_2000[5], Ro_smt_400[5], Ro_smt_2000[5] | Название привязано к 5 Tx |
| `DAT` | 232 байта | Устаревший прототип | Не использовалось |

---

## 2. Новые структуры (`variable.h`)

### 2.1. `GP_TOOL_DATA` (240 байт, `#pragma pack(push, 1)`)

```c
struct GP_TOOL_DATA {
    uint32_t signature;
    uint16_t condition;
    uint16_t frame;
    uint16_t unused;
    float temperature;
    float DELTA_PH[2][5];      // freq-major: [0]=400 кГц, [1]=2000 кГц
    float AM_RX_1[2][5];
    float AM_RX_2[2][5];
    float ZERO_AM[2][5];
    float rho_smt[2][5];
    uint32_t raw_frequency[2];
    uint32_t tmp_frequency[2];
    float temperature_dds[2];
};
```

### 2.2. `GP_TOOL_METROLOGY` (240 байт, `#pragma pack(push, 1)`)

```c
struct GP_TOOL_METROLOGY {
    uint32_t signature;
    uint16_t serial;
    uint16_t condition;
    uint16_t frame;
    uint16_t padding;
    float F[2];                // [0]=400 кГц, [1]=2000 кГц
    int16_t L1[5];
    int16_t L2[5];
    float Air_zz[2][5];        // 2D: [freq][Tx]
    float Air_zz_amt[2][5];
    int32_t PhaseOffsets[2][10];
    uint16_t CRC;
    uint16_t mCrc;
};
```

### 2.3. Сохранённые структуры
- `V10` (40 байт, `VALUE[10]`) — универсальный контейнер для данных
- `INC_SET` — параметры калибровки
- `ARGB` — цветовая модель
- `SONDE_PARAM` — параметры зонда
- `LOOP_PARAM` — параметры кольца

---

## 3. Индексация (freq-major, N_Tx из сигнатуры)

### 3.1. Определение N_Tx

```c
uint32_t buff = (signature << 12) >> 12;
uint32_t N_Tx = (buff % 100000) / 10000;  // toolCode, средняя цифра
// Допустимый диапазон: 3..5. Если вне диапазона — fallback N_Tx = 5.
```

### 3.2. Раскладка V10

Линейный индекс: `VALUE[freq * N_Tx + Tx]`
- `freq = 0` → 400 кГц
- `freq = 1` → 2000 кГц
- `Tx = 0..N_Tx-1` → передатчики

Эта раскладка должна совпадать с порядком `sonde_param`, который формируется в `metro_convert` (freq-major, страйд = N_Tx).

---

## 4. Изменения по файлам

### 4.1. `function.cpp`

| Функция | Изменения |
|---------|-----------|
| `metro_convert` | Чтение `GP_TOOL_METROLOGY` из `void*`; N_Tx из signature; цикл `for(freq=0..1) for(Tx=0..N_Tx-1)`; заполнение `sonde_param` в freq-major порядке |
| `get_ph_from_SDR` | Чтение `GP_TOOL_DATA` из COM-порта; извлечение `DELTA_PH[freq][Tx]`; sign-flip для чётных индексов сохранён |
| `get_ph_from_1DDS` | То же: чтение `GP_TOOL_DATA`, `DELTA_PH[freq][Tx]`; нормализация `[-PI,PI]`; sign-flip |
| `get_ph_from_AUTONOM` | То же; DLL `AMK_GetData` пишет в `GP_TOOL_DATA` (считаем корректным) |
| `calc_phase_from_loop` | Аргумент `GP_TOOL_METROLOGY*`; `V10` возврат; индексация freq-major; математика без изменений |
| `SIGNAL` | Аргумент `GP_TOOL_METROLOGY*`; маппинг `n → (freq = n/N_Tx, Tx = n%N_Tx)`; частота берётся из `F[freq]`; длины из `L1[Tx]/L2[Tx]` |
| `RO` / `R0_adecvat_r_loop` | Аргументы `GP_TOOL_METROLOGY*` и `V10*`; N_Tx из signature |
| `get_phase_raw` | Аргумент `GP_TOOL_DATA*`; чтение `DELTA_PH[freq][Tx]`; нормализация `[-PI,PI]` |
| `get_phase` | То же + нормализация к воздуху через `Air_zz[freq][Tx]`; sign-flip для чётных индексов |
| `get_phase_smt` | Симметризация через матрицу K[4][4] для первых 4 передатчиков; freq-major раскладка |
| `get_air_smt` | То же; работа с `V10*` |
| `READ_BLOK` | Переименована в `READ_BLOK_GP_TOOL_DATA`; чтение `GP_TOOL_DATA` из файла |

### 4.2. `function.h`

Все прототипы обновлены: `GP_TOOL_DATA*`, `GP_TOOL_METROLOGY*`, `V10` вместо `METROLOGY*`, `AllData_1DDS*`, `V8`.

### 4.3. `Lopp.cpp` (глобальные переменные)

| Переменная | Было | Стало |
|------------|------|-------|
| `metrology_struct` | `METROLOGY` | `GP_TOOL_METROLOGY` |
| `Temp_data` | `vector<AllData_1DDS>` | `vector<GP_TOOL_DATA>` |
| `Air_Phase` | `V8` | `V10` |
| `Ro_percent_temperature` | `V8[200][10]` | `V10[200][10]` |
| `metrology_struct_for_autonom` | `METROLOGY_FOR_AUTONOM` | **Удалена** (не использовалась) |

### 4.4. `MetroForm.h`

- Тип `METROLOGY` → `GP_TOOL_METROLOGY` везде.
- Доступ к `Air_zz` адаптирован: `Air_zz[i]` → `Air_zz[i/5][i%5]` (линейные индексы 0..9 маппятся на `[freq][Tx]`).
- Частоты: `F[8]` → `F[2]`. Убраны legacy-ссылки `F[4]`, `F[i+3]` (были артефактами старой раскладки).
- Чтение/запись метрологии в файл: `sizeof(metrology_struct)` = 240 байт.

### 4.5. `WorkForm.h`

- Типы `METROLOGY` / `METROLOGY_NEW` → `GP_TOOL_METROLOGY`.
- Debug-кнопка `button_calk`: тип указателя `METROLOGY_NEW*` → `GP_TOOL_METROLOGY*`.
- Закомментированный debug-блок: `V8` → `V10`.

### 4.6. `MyForm1.h`

- `extern METROLOGY metrology_struct` → `extern GP_TOOL_METROLOGY metrology_struct`.

---

## 5. Сохранённая логика (не изменялась)

- **Ring-математика**: `dfi_from_loop`, `ro_dfi`, `calc_PH_from_loop`, `calc_Ro_from_loop`, `Z()`, `Loop()`, `Loop_Re()`, `C_max_fi()`, `F()`, `C_f()` — формулы и алгоритмы не тронуты.
- **Sign-flip**: инверсия знака для чётных индексов (зеркальная геометрия Z1 повёрнут T1) сохранена в `get_ph_from_*` и `get_phase*`.
- **Нормализация фаз**: приведение к `[-PI, PI]` через `while(value > PI) value -= 2*PI` сохранена.
- **Симметризация**: матрица `K[4][4]` в `get_phase_smt` и `get_air_smt` не изменена.
- **Константы**: `PI`, `mu0`, `mG`, `mV` — без изменений.
- **COM-команды**: `CMD_WORK`, `CMD_GET_DATA`, `CMD_GET_DATA_5_Tx` — без изменений.
- **DLL**: `AMK_1DDS_AUTONOM.dll` и `AMK_GetData` — не трогались.

---

## 6. COM-протокол и взаимодействие с прибором

### 6.1. Команды (из `variable.h`)

| Команда | Значение | Назначение |
|---------|----------|------------|
| `CMD_GET_DATA` | `0x01 \| KEY` (0x11) | Получить урезанный кадр (SDR) |
| `CMD_GET_DATA_5_Tx` | `0x05 \| KEY` (0x15) | Получить кадр 5 Tx |
| `CMD_WORK` | `0x07 \| ADDRESS` (0x67 / 0x77, зависит от ADDRESS) | Рабочий режим |

### 6.2. Длины пакетов

| Источник | Длина данных | Примечание |
|----------|-------------|------------|
| SDR (`CMD_GET_DATA`) | 242 байта = `sizeof(GP_TOOL_DATA) + 2` (CRC) | Через `receive_from_tool`, смещение +2 байта |
| 1DDS (`CMD_WORK`) | 252 байта, метрология со смещением +12 | `(GP_TOOL_DATA*)(DATA_IN + 12)` |
| AUTONOM (DLL) | `AMK_GetData(void* ComPort, void* Data)` | DLL пишет `GP_TOOL_DATA` |

### 6.3. Формат метрологии

Файл `Metrology.bin` — бинарный дамп `GP_TOOL_METROLOGY` (240 байт). Читается/пишется через `ifstream::read`/`ofstream::write` с `sizeof(metrology_struct)`.

---

## 7. Ссылки на методические документы (для проекта Amk-Horizon-New-Chart)

Для понимания актуальных структур данных и взаимодействия прибора с приложением (COM-порт) в смежном проекте **Amk-Horizon-New-Chart** следует ссылаться на следующие источники:

### 7.1. Внутренние источники текущего проекта (LOOP_NEW)

| Документ / Файл | Что описывает |
|-----------------|---------------|
| `variable.h` | Полное определение `GP_TOOL_DATA` и `GP_TOOL_METROLOGY` с полями, типами и размерами |
| `function.h` | Прототипы всех функций обработки данных: сигнатуры, типы аргументов, возвращаемые значения |
| `function.cpp` | Реализация: как извлекаются `DELTA_PH`, как применяется sign-flip, как происходит индексация, математика кольца |
| `MetroForm.h` | UI-логика чтения/записи метрологии: как `Air_zz[2][5]` маппится на линейные индексы 0..9 |

### 7.2. Внешние методические документы (ожидаются от разработчика firmware / embedded)

| Документ | Содержание | Где получить |
|----------|------------|--------------|
| **Спецификация telemetry-фрейма GP_TOOL** | Точное binary-layout полей `GP_TOOL_DATA` (порядок байт, padding, выравнивание), значения condition-flags, формат signature | Разработчик firmware прибора |
| **Спецификация метрологии GP_TOOL_METROLOGY** | Layout полей калибровки, формат `Air_zz`, `PhaseOffsets`, правила расчёта CRC | Разработчик firmware прибора |
| **Протокол обмена по COM-порту** | Таблица команд, формат пакетов request/response, таймауты, baudrate, процедура инициализации | Разработчик firmware / документация прибора |
| **Спецификация DLL `AMK_1DDS_AUTONOM.dll`** | Экспортируемые функции (`AMK_GetData`, `AMK_StartComPort`, `AMK_Init` и др.), ожидаемые типы данных, размер структуры, которую пишет DLL | Разработчик DLL |

### 7.3. Важные конвенции для Amk-Horizon-New-Chart

1. **N_Tx из сигнатуры**: универсальный способ определения количества передатчиков. Не хардкодить 5 или 8.
2. **Freq-major индексация**: все массивы результатов (`V10.VALUE`, `sonde_param`, `DELTA_PH`) укладываются как `[freq * N_Tx + Tx]`.
3. **F[2]**: две рабочие частоты (400 кГц и 2000 кГц). Не путать со старым `F[8]`.
4. **Air_zz[2][5]**: фаза в воздухе — 2D-массив `[freq][Tx]`. UI маппит линейные индексы 0..9 на `[i/5][i%5]`.
5. **Sign-flip**: чётные индексы (Tx=0,2,4...) инвертируются. Это геометрия прибора (зеркальные передатчики).

---

## 8. Замечания и риски

1. **Мёртвый код**: `MyForm2.h` / `MyForm3.h` содержат ссылки на старую модель (`Temp_data[n].TX[...]`) и не компилируются. При необходимости активации — потребуется отдельный рефакторинг.
2. **Размер memcpy в MetroForm.h**: `memcpy(&metrology_struct, DATA_IN, 256)` копирует 256 байт в 240-байтную структуру. Было и в старом коде. **Рекомендация**: заменить на `sizeof(GP_TOOL_METROLOGY)` (240) при отладке.
3. **DLL**: предполагается, что `AMK_1DDS_AUTONOM.dll` и `AMK_GetData` возвращают данные в формате `GP_TOOL_DATA` (240 байт). Если DLL возвращает старый формат — потребуется адаптация DLL или промежуточный маппинг.
4. **Сборка**: проект C++/CLI, toolset v145. Потребуется полная перекомпиляция.
5. **UI Air**: форма MetroForm отображает 10 текстбоксов (`textBoxAIRT1..AIRT10`) для `Air_zz`. При `N_Tx < 5` часть боксов будет неактуальна — UI не скрывает лишние поля (legacy-особенность).

---

## 10. Система логирования (добавлено 2026-06-15)

Для тестирования с эмулятором прибора добавлена подробная система логирования в файл `LoopDeviceLog.txt`.

### 10.1. Функция логирования

```cpp
inline void WriteLog(const string& message) {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    ofstream log_file("LoopDeviceLog.txt", ios::app);
    log_file << "[" << timestamp << "] " << message << endl;
    // Также вывод в cout для отладки в IDE
}
```

### 10.2. Логируемые события при включении прибора (button_ON_OFF_Click)

#### Для AUTONOM (DLL):
- Загрузка DLL `AMK_1DDS_AUTONOM.dll`
- Получение адресов функций из DLL
- Открытие COM-порта (`AMK_StartComPort`)
- Установка режима IDLE
- Включение питания (`AMK_PwrOn`)
- Инициализация прибора (`AMK_Init`)
- Получение метрологии (`AMK_GetMetroData`) - 3 попытки
- **Содержимое метрологии:**
  - Signature (hex)
  - Serial number
  - N_Tx (вычисленный из signature)
  - Частоты F[0], F[1]
  - Параметры зондов L1[5], L2[5]
  - Воздушные поправки Air_zz[2][5]
- Конвертация в sonde_param
- Запуск циклограммы (`AMK_BeginCyclogram`)
- Результат включения (успех/ошибка)

#### Для SDR:
- Установка BaudRate = 115200
- Открытие COM-порта
- Чтение метрологии (команда 0x13)
- Содержимое метрологии (signature, N_Tx, частоты)
- Результат включения

#### Для LWD1DDS:
- Установка BaudRate = 125000
- Открытие COM-порта
- Чтение данных (`get_ph_from_1DDS`)
- Чтение метрологии из EEPROM
- Serial number из EEPROM
- Содержимое метрологии
- Результат включения

### 10.3. Логируемые данные при опросе прибора (sonde_read_timer)

Каждую секунду (период таймера):
- Тип прибора и вызываемая функция чтения
- Результат чтения (res: 0=успех, другое=ошибка)
- Номер кадра (global_cntr)
- Размер sonde_param
- **Полученные фазы (ph_data.VALUE):**
  - Для каждого канала: индекс, частота (400/2000 kHz), номер Tx
  - Значение в радианах и миллиградусах (mG)
- **Вычисленное УЭС (ro_data.VALUE):**
  - Для каждого канала: индекс, частота, номер Tx
  - Значение в Ohm*m

### 10.4. Формат лог-файла

```
[HH:MM:SS] === НАЧАЛО ВКЛЮЧЕНИЯ АВТОНОМНИКА ===
[HH:MM:SS] Тип прибора: AUTONOM
[HH:MM:SS] DLL AMK_1DDS_AUTONOM.dll загружена успешно
[HH:MM:SS] Адреса функций получены
[HH:MM:SS] Инициализация COM-порта...
[HH:MM:SS] ComPort открыт, handle: 0x...
...
[HH:MM:SS] --- Получение данных от прибора ---
[HH:MM:SS] Номер кадра: 15, размер sonde_param: 10
[HH:MM:SS] Полученные фазы (ph_data.VALUE):
[HH:MM:SS]   [0] freq=400kHz Tx=0: 0.001234 rad (70.5 mG)
...
```

### 10.5. Использование для отладки

1. Перед тестированием удалите старый `LoopDeviceLog.txt` для чистоты
2. Запустите приложение
3. Нажмите "ВКЛЮЧИТЬ ПРИБОР"
4. Проверьте лог-файл в каталоге приложения
5. При работе с эмулятором каждый кадр будет записан с данными

**Примечание:** При длительной работе файл лога может стать большим. Для продакшена рекомендуется ограничить логирование только ошибками или добавить ротацию.

---

## 11. История изменений

| Дата | Автор | Описание |
|------|-------|----------|
| 2026-06-03 | Cascade (AI) | Полный рефакторинг: замена структур, адаптация всех функций и форм, документация |
| 2026-06-15 | Cascade (AI) | Добавлена система логирования для тестирования с эмулятором прибора |
