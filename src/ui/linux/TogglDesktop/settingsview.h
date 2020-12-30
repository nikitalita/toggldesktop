// Copyright 2014 Toggl Desktop developers.

#ifndef SRC_UI_LINUX_TOGGLDESKTOP_SETTINGSVIEW_H_
#define SRC_UI_LINUX_TOGGLDESKTOP_SETTINGSVIEW_H_

#include <QObject>
#include <QTime>

#include "toggl.h"

class SettingsView : public QObject {
    Q_OBJECT

 public:
    explicit SettingsView(QObject *parent = 0);

    static SettingsView *importOne(TogglSettingsView *view) {
        SettingsView *result = new SettingsView();
        result->AutodetectProxy = view->AutodetectProxy;
        result->UseProxy = view->UseProxy;
        result->ProxyHost = toQString(view->ProxyHost);
        result->ProxyPort = view->ProxyPort;
        result->ProxyUsername = toQString(view->ProxyUsername);
        result->ProxyPassword = toQString(view->ProxyPassword);
        result->UseIdleDetection = view->UseIdleDetection;
        result->MenubarTimer = view->MenubarTimer;
        result->DockIcon = view->DockIcon;
        result->OnTop = view->OnTop;
        result->Reminder = view->Reminder;
        result->RecordTimeline = view->RecordTimeline;
        result->IdleMinutes = view->IdleMinutes;
        result->ReminderMinutes = view->ReminderMinutes;
        result->ManualMode = view->ManualMode;
        result->FocusOnShortcut = view->FocusOnShortcut;
        result->Pomodoro = view->Pomodoro;
        result->PomodoroMinutes = view->PomodoroMinutes;
        result->PomodoroBreak = view->PomodoroBreak;
        result->PomodoroBreakMinutes = view->PomodoroBreakMinutes;
        result->RemindOnMonday = view->RemindMon;
        result->RemindOnTuesday = view->RemindTue;
        result->RemindOnWednesday = view->RemindWed;
        result->RemindOnThursday = view->RemindThu;
        result->RemindOnFriday = view->RemindFri;
        result->RemindOnSaturday = view->RemindSat;
        result->RemindOnSunday = view->RemindSun;
        result->RemindStartTime = QTime::fromString(toQString(view->RemindStarts), "HH:mm");
        result->RemindEndTime = QTime::fromString(toQString(view->RemindEnds), "HH:mm");
        result->StopEntryOnShutdownSleep = view->StopEntryOnShutdownSleep;
        result->AnalyticsOptedOut = view->AnalyticsOptedOut;
        result->ForceIgnoreCert = view->ForceIgnoreCert;
        return result;
    }

    bool UseProxy;
    QString ProxyHost;
    uint64_t ProxyPort;
    QString ProxyUsername;
    QString ProxyPassword;
    bool UseIdleDetection;
    bool MenubarTimer;
    bool DockIcon;
    bool OnTop;
    bool Reminder;
    bool RecordTimeline;
    uint64_t IdleMinutes;
    uint64_t ReminderMinutes;
    bool ManualMode;
    bool AutodetectProxy;
    bool FocusOnShortcut;
    bool Pomodoro;
    uint64_t PomodoroMinutes;
    bool PomodoroBreak;
    uint64_t PomodoroBreakMinutes;
    bool RemindOnMonday;
    bool RemindOnTuesday;
    bool RemindOnWednesday;
    bool RemindOnThursday;
    bool RemindOnFriday;
    bool RemindOnSaturday;
    bool RemindOnSunday;
    QTime RemindStartTime;
    QTime RemindEndTime;
    bool StopEntryOnShutdownSleep;
    bool AnalyticsOptedOut;
    bool ForceIgnoreCert;
};

#endif  // SRC_UI_LINUX_TOGGLDESKTOP_SETTINGSVIEW_H_
