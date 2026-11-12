# CUNC Student List

A Qt Widgets desktop application for maintaining a student list, with QR-code and Telegram-sending interfaces.

## Requirements

- Qt 6 with Widgets and Network modules
- A C++17 compiler
- qmake 6 and make

## Build

```sh
mkdir -p build
cd build
qmake6 ../cunc_student_list1.pro
make -j"$(nproc)"
```

The application currently has no automated test suite. The source was validated with a clean out-of-tree Qt 6 build.

## Configuration

Telegram credentials are passed into `TelegramSenderWidget` through its constructor; do not hard-code bot tokens or commit local configuration files.

Note: the QR-code widget currently has a low-probability Osidect promotional override. Remove that override if every QR code must always represent the supplied student data.
