// Copyright 2014 Toggl Desktop developers.
#include <QDebug>  // NOLINT

#include "./preferencesdialog.h"
#include "./ui_preferencesdialog.h"

#include "./toggl.h"
#include "./settingsview.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent),
ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);

    connect(TogglApi::instance, SIGNAL(displaySettings(bool,SettingsView*)),  // NOLINT
            this, SLOT(displaySettings(bool,SettingsView*)));  // NOLINT

    connect(TogglApi::instance, SIGNAL(displayLogin(bool,uint64_t)),  // NOLINT
            this, SLOT(displayLogin(bool,uint64_t)));  // NOLINT

    connect(TogglApi::instance, SIGNAL(updateShowHideShortcut()),  // NOLINT
            this, SLOT(updateShowHideShortcut()));  // NOLINT

    connect(TogglApi::instance, SIGNAL(updateContinueStopShortcut()),  // NOLINT
            this, SLOT(updateContinueStopShortcut()));  // NOLINT

    connect(ui->dayCheckbox_1, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->dayCheckbox_2, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->dayCheckbox_3, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->dayCheckbox_4, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->dayCheckbox_5, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->dayCheckbox_6, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->dayCheckbox_7, &QCheckBox::clicked, this, &PreferencesDialog::onDayCheckboxClicked);
    connect(ui->stopEntry, &QCheckBox::clicked, this, &PreferencesDialog::onStopEntryCheckboxClicked);
    keyId = 0;
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::setBeginningOfWeek(uint8_t day) {
    if (day == beginningOfWeek)
        return;
    QVector<bool> stored;
    for (int i = 0; i < 7; i++)
        stored.append(checkBoxForDay(i)->isChecked());
    beginningOfWeek = day;
    for (int i = 0; i < 7; i++)
        checkBoxForDay(i)->setChecked(stored[i]);
    updateCheckboxLabels();
}

void PreferencesDialog::displaySettings(const bool open,
                                        SettingsView *settings) {

    setBeginningOfWeek(TogglApi::instance->getUserBeginningOfWeek());

    if (open) {
        bool wasVisible = isVisible();
        show();
        if (!wasVisible)
            ui->tabWidget->setCurrentIndex(0);
    }

    ui->useSystemProxySettings->setChecked(settings->AutodetectProxy);

    ui->useProxy->setChecked(settings->UseProxy);
    ui->proxyHost->setText(settings->ProxyHost);
    ui->proxyPort->setText(QString::number(settings->ProxyPort));
    ui->proxyUsername->setText(settings->ProxyUsername);
    ui->proxyPassword->setText(settings->ProxyPassword);

    ui->recordTimeline->setChecked(settings->RecordTimeline);  // user based!

    ui->idleDetection->setChecked(settings->UseIdleDetection);
    ui->idleMinutes->setText(QString::number(settings->IdleMinutes));
    ui->idleMinutes->setEnabled(ui->idleDetection->isChecked());

    ui->remindToTrackTime->setChecked(settings->Reminder);
    ui->reminderMinutes->setText(QString::number(settings->ReminderMinutes));
    ui->reminderMinutes->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_1->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_2->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_3->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_4->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_5->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_6->setEnabled(ui->remindToTrackTime->isChecked());
    ui->dayCheckbox_7->setEnabled(ui->remindToTrackTime->isChecked());
    ui->reminderStartTimeEdit->setEnabled(ui->remindToTrackTime->isChecked());
    ui->reminderEndTimeEdit->setEnabled(ui->remindToTrackTime->isChecked());

    ui->pomodoroTimer->setChecked(settings->Pomodoro);
    ui->pomodoroMinutes->setText(QString::number(settings->PomodoroMinutes));
    ui->pomodoroMinutes->setEnabled(ui->pomodoroTimer->isChecked());

    ui->pomodoroBreakTimer->setChecked(settings->PomodoroBreak);
    ui->pomodoroBreakTimer->setEnabled(settings->Pomodoro);
    ui->pomodoroBreakMinutes->setText(
        QString::number(settings->PomodoroBreakMinutes));
    ui->pomodoroBreakMinutes->setEnabled(
        ui->pomodoroTimer->isChecked()
        && ui->pomodoroBreakTimer->isChecked());

    ui->focusAppOnShortcut->setChecked((settings->FocusOnShortcut));

    checkBoxForDay(1)->setChecked(settings->RemindOnMonday);
    checkBoxForDay(2)->setChecked(settings->RemindOnTuesday);
    checkBoxForDay(3)->setChecked(settings->RemindOnWednesday);
    checkBoxForDay(4)->setChecked(settings->RemindOnThursday);
    checkBoxForDay(5)->setChecked(settings->RemindOnFriday);
    checkBoxForDay(6)->setChecked(settings->RemindOnSaturday);
    checkBoxForDay(0)->setChecked(settings->RemindOnSunday);

    ui->reminderStartTimeEdit->setTime(settings->RemindStartTime);
    ui->reminderEndTimeEdit->setTime(settings->RemindEndTime);

    ui->stopEntry->setChecked(settings->StopEntryOnShutdownSleep);

    QString sh(TogglApi::instance->getShowHideKey());
    if (sh.length() == 0) {
        sh = "Record shortcut";
    }
    ui->showHideButton->setText(sh);

    QString cs(TogglApi::instance->getContinueStopKey());
    if (cs.length() == 0) {
        cs = "Record shortcut";
    }
    ui->continueStopButton->setText(cs);

    ui->sslCheckbox->setChecked(settings->ForceIgnoreCert);
}

void PreferencesDialog::displayLogin(const bool open,
                                     const uint64_t user_id) {
    ui->recordTimeline->setEnabled(!open && user_id);
}

void PreferencesDialog::onDayCheckboxClicked(bool checked) {
    Q_UNUSED(checked);
    TogglApi::instance->setSettingsRemindDays(
        checkBoxForDay(1)->isChecked(),
        checkBoxForDay(2)->isChecked(),
        checkBoxForDay(3)->isChecked(),
        checkBoxForDay(4)->isChecked(),
        checkBoxForDay(5)->isChecked(),
        checkBoxForDay(6)->isChecked(),
        checkBoxForDay(0)->isChecked()
    );
}

void PreferencesDialog::on_proxyHost_editingFinished() {
    setProxySettings();
}

void PreferencesDialog::on_proxyPort_editingFinished() {
    setProxySettings();
}

void PreferencesDialog::on_proxyUsername_editingFinished() {
    setProxySettings();
}

void PreferencesDialog::on_proxyPassword_editingFinished() {
    setProxySettings();
}

void PreferencesDialog::on_idleDetection_clicked(bool checked) {
    TogglApi::instance->setSettingsUseIdleDetection(checked);
}

void PreferencesDialog::on_recordTimeline_clicked(bool checked) {
    TogglApi::instance->toggleTimelineRecording(checked);
}

void PreferencesDialog::on_remindToTrackTime_clicked(bool checked) {
    TogglApi::instance->setSettingsReminder(checked);
}

void PreferencesDialog::on_pomodoroTimer_clicked(bool checked) {
    TogglApi::instance->setSettingsPomodoro(checked);
}

void PreferencesDialog::on_pomodoroBreakTimer_clicked(bool checked) {
    TogglApi::instance->setSettingsPomodoroBreak(checked);
}

void PreferencesDialog::updateShowHideShortcut() {
    QString text(TogglApi::instance->getShowHideKey());
    if (text.length() == 0) {
        text = "Record shortcut";
    }
    ui->showHideButton->setText(text);
}

void PreferencesDialog::updateContinueStopShortcut() {
    QString text(TogglApi::instance->getContinueStopKey());
    if (text.length() == 0) {
        text = "Record shortcut";
    }
    ui->continueStopButton->setText(text);
}

void PreferencesDialog::updateCheckboxLabels() {
    static const QVector<QString> days { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    for (int i = 0; i < 7; i++) {
        checkBoxForDay(i)->setText(days[i]);
    }
}

void PreferencesDialog::on_showHideClear_clicked() {
    keySequence = "";
    keyId = 1;
    saveCurrentShortcut();
}

void PreferencesDialog::on_continueStopClear_clicked() {
    keySequence = "";
    keyId = 2;
    saveCurrentShortcut();
}

void PreferencesDialog::on_showHideButton_clicked() {
    ui->showHideButton->setText("Type shortcut");
    keyId = 1;
}

void PreferencesDialog::on_reminderStartTimeEdit_editingFinished() {
    TogglApi::instance->setSettingsRemindTimes(
        ui->reminderStartTimeEdit->time(),
        ui->reminderEndTimeEdit->time()
    );
}

void PreferencesDialog::on_reminderEndTimeEdit_editingFinished() {
    TogglApi::instance->setSettingsRemindTimes(
        ui->reminderStartTimeEdit->time(),
        ui->reminderEndTimeEdit->time()
                );
}

void PreferencesDialog::on_sslCheckbox_clicked() {
    TogglApi::instance->setSettingsIgnoreCert(ui->sslCheckbox->isChecked());
}

void PreferencesDialog::on_continueStopButton_clicked() {
    ui->continueStopButton->setText("Type shortcut");
    keyId = 2;
}

void PreferencesDialog::keyPressEvent(QKeyEvent *event) {
    if (keyId) {
        keySequence = "";
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);
        if (key == Qt::Key_unknown) {
            return;
        }

        // Reset to previous values
        if (key == Qt::Key_Escape) {
            if (keyId == 1) {
                TogglApi::instance->updateShowHideShortcut();
            }
            if (keyId == 2) {
                TogglApi::instance->updateContinueStopShortcut();
            }
            keyId = 0;
            return;
        }

        // the user have clicked just and
        // only the special keys Ctrl, Shift, Alt, Meta.
        if (key == Qt::Key_Control ||
                key == Qt::Key_Shift ||
                key == Qt::Key_Alt ||
                key == Qt::Key_Meta) {
            return;
        }

        // check for a combination of user clicks
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();

        if (modifiers & Qt::ShiftModifier)
            keyInt += Qt::SHIFT;
        if (modifiers & Qt::ControlModifier)
            keyInt += Qt::CTRL;
        if (modifiers & Qt::AltModifier)
            keyInt += Qt::ALT;
        if (modifiers & Qt::MetaModifier)
            keyInt += Qt::META;

        keySequence = QKeySequence(keyInt).toString(QKeySequence::NativeText);

        if (keyId == 1) {
            ui->showHideButton->setText(keySequence);
        } else if (keyId == 2) {
            ui->continueStopButton->setText(keySequence);
        }
    }
    else {
        if (event->key() == Qt::Key_Escape) {
            close();
        }
    }
}

void PreferencesDialog::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    saveCurrentShortcut();
}

void PreferencesDialog::saveCurrentShortcut() {
    if (keyId == 1) {
        TogglApi::instance->setShowHideKey(keySequence);
    } else if (keyId == 2) {
        TogglApi::instance->setContinueStopKey(keySequence);
    }

    keyId = 0;
    keySequence = "";
}

bool PreferencesDialog::setProxySettings() {
    return TogglApi::instance->setProxySettings(ui->useProxy->isChecked(),
            ui->proxyHost->text(),
            ui->proxyPort->text().toULongLong(),
            ui->proxyUsername->text(),
                                                ui->proxyPassword->text());
}

QCheckBox *PreferencesDialog::checkBoxForDay(uint8_t day) {
    int actualDay = day - beginningOfWeek;
    actualDay = actualDay < 0 ? 7 + actualDay : actualDay;
    switch (actualDay) {
    case 0: return ui->dayCheckbox_1;
    case 1: return ui->dayCheckbox_2;
    case 2: return ui->dayCheckbox_3;
    case 3: return ui->dayCheckbox_4;
    case 4: return ui->dayCheckbox_5;
    case 5: return ui->dayCheckbox_6;
    case 6: return ui->dayCheckbox_7;
    default: return nullptr;
    }
}

void PreferencesDialog::on_useProxy_clicked(bool checked) {
    Q_UNUSED(checked);
    setProxySettings();
}

void PreferencesDialog::on_idleMinutes_editingFinished() {
    TogglApi::instance->setSettingsIdleMinutes(
        ui->idleMinutes->text().toULongLong());
}

void PreferencesDialog::on_reminderMinutes_editingFinished() {
    TogglApi::instance->setSettingsReminderMinutes(
        ui->reminderMinutes->text().toULongLong());
}

void PreferencesDialog::on_pomodoroMinutes_editingFinished() {
    TogglApi::instance->setSettingsPomodoroMinutes(
        ui->pomodoroMinutes->text().toULongLong());
}

void PreferencesDialog::on_pomodoroBreakMinutes_editingFinished() {
    TogglApi::instance->setSettingsPomodoroBreakMinutes(
        ui->pomodoroBreakMinutes->text().toULongLong());
}

void PreferencesDialog::on_useSystemProxySettings_clicked(bool checked) {
    TogglApi::instance->setSettingsAutodetectProxy(checked);
}

void PreferencesDialog::on_focusAppOnShortcut_clicked(bool checked) {
    TogglApi::instance->setSettingsFocusOnShortcut(checked);
}

void PreferencesDialog::onStopEntryCheckboxClicked(bool checked) {
    TogglApi::instance->setSettingsStopEntryOnShutdown(checked);
}

void PreferencesDialog::onAnalyticsOptedOutCheckboxClicked(bool checked) {
    TogglApi::instance->setSettingsAnalyticsOptedOut(checked);
}