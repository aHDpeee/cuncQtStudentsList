#include "table.h"
#include "mytime.h"
#include <QApplication>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>

// Функция убивает только старые процессы с тем же именем, не себя
void killExistingProcess(const QString& processName) {
    DWORD currentPID = GetCurrentProcessId(); // PID текущего процесса
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return;

    if (Process32First(snapshot, &entry) == TRUE) {
        do {
            DWORD pid = entry.th32ProcessID;
            if (pid == currentPID) continue; // пропускаем себя

            QString exe = QString::fromWCharArray(entry.szExeFile);
            if (exe.compare(processName, Qt::CaseInsensitive) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    qDebug() << "Killed process:" << processName << "PID:" << pid;
                }
            }
        } while (Process32Next(snapshot, &entry) == TRUE);
    }

    CloseHandle(snapshot);
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_OS_WIN
    QString exeName = "cunc_student_list1_boxed.exe";
    killExistingProcess(exeName); // убиваем только прошлый процесс, не себя
#endif

    a.setQuitOnLastWindowClosed(true);

    // Запуск интерфейса
    Table* w = new Table();
    w->show();

    MyTime* t = new MyTime();
    t->show();

    return a.exec();
}
