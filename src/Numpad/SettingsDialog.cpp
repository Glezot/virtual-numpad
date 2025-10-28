/*
 * This source file is part of Numpad.
 * Copyright (C) 2013 Timur Abdrazyakov
 * abtimal@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */


#include "SettingsDialog.h"
#include "Numpad.h"
#include "NumpadManager.h"
#include "ShowHideKey.h"
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFontMetrics>
#include <QPushButton>
#include <QGridLayout>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPoint>
#include <QString>
#include <vector>
#include <Windows.h>


namespace
{
int parsePositionValue(QLineEdit *lineEdit, int fallback)
{
    if (!lineEdit)
    {
        return fallback;
    }

    bool ok = false;
    const int value = lineEdit->text().toInt(&ok);
    if (!ok)
    {
        lineEdit->setText(QString::number(fallback));
        return fallback;
    }

    lineEdit->setText(QString::number(value));
    return value;
}
}

SettingsDialog::SettingsDialog(NumpadManager *p_numpadManager, Numpad *p_numpad,
                               int initBtnSize, int initSpacing, QWidget *p_wid/*= 0*/)
: QWidget(p_wid, Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
  pm_numpadManager(p_numpadManager),
  pm_numpad(p_numpad)
{
  setWindowTitle("Settings");

  QCheckBox *p_showGearCheckBox = new QCheckBox("Show gear on keyboard");
  p_showGearCheckBox->setChecked(pm_numpadManager->readMenuVisibleFromSettings());

  QVBoxLayout *p_showGearLayout = new QVBoxLayout;
  p_showGearLayout->addWidget(p_showGearCheckBox);
  p_showGearLayout->addWidget(new QLabel("(Menu always available in system tray)"));
  p_showGearLayout->addStretch(1);
  QGroupBox *p_showGearGrpBox = new QGroupBox;
  p_showGearGrpBox->setLayout(p_showGearLayout);

  QCheckBox *p_showLayoutBtnCheckBox = new QCheckBox("Show layout switch button");
  p_showLayoutBtnCheckBox->setChecked(pm_numpadManager->readLayoutBtnVisibleFromSettings());
  QHBoxLayout *p_layoutBtnLayout = new QHBoxLayout;
  p_layoutBtnLayout->addWidget(p_showLayoutBtnCheckBox);
  p_layoutBtnLayout->addStretch(1);
  QGroupBox *p_layoutBtnGrpBox = new QGroupBox;
  p_layoutBtnGrpBox->setLayout(p_layoutBtnLayout);

  QCheckBox *p_autoRunCheckBox = new QCheckBox("Autorun with Windows");
  p_autoRunCheckBox->setChecked(pm_numpadManager->isAutoRunSet());

  QHBoxLayout *p_horAutoRunLayout = new QHBoxLayout;
  p_horAutoRunLayout->addWidget(p_autoRunCheckBox);
  p_horAutoRunLayout->addStretch(1);
  QGroupBox *p_autoRunGrpBox = new QGroupBox;
  p_autoRunGrpBox->setLayout(p_horAutoRunLayout);

  pm_rememberPositionCheckBox = new QCheckBox("Remember last window position");
  pm_rememberPositionCheckBox->setChecked(pm_numpadManager->readRememberLastPositionFromSettings());

  const QPoint initialPosition = pm_numpadManager->readInitialPositionFromSettings();

  pm_initialPosXLineEdit = new QLineEdit;
  pm_initialPosXLineEdit->setValidator(new QIntValidator(-10000, 10000, pm_initialPosXLineEdit));
  pm_initialPosXLineEdit->setText(QString::number(initialPosition.x()));

  pm_initialPosYLineEdit = new QLineEdit;
  pm_initialPosYLineEdit->setValidator(new QIntValidator(-10000, 10000, pm_initialPosYLineEdit));
  pm_initialPosYLineEdit->setText(QString::number(initialPosition.y()));

  pm_applyPositionButton = new QPushButton("Move numpad to position");
  pm_pickCurrentPositionButton = new QPushButton("Use current position");

  QLabel *p_posXLbl = new QLabel("X:");
  QLabel *p_posYLbl = new QLabel("Y:");

  QGridLayout *p_positionGridLayout = new QGridLayout;
  p_positionGridLayout->addWidget(p_posXLbl, 0, 0);
  p_positionGridLayout->addWidget(pm_initialPosXLineEdit, 0, 1);
  p_positionGridLayout->addWidget(p_posYLbl, 1, 0);
  p_positionGridLayout->addWidget(pm_initialPosYLineEdit, 1, 1);
  p_positionGridLayout->addWidget(pm_pickCurrentPositionButton, 2, 0, 1, 2);
  p_positionGridLayout->addWidget(pm_applyPositionButton, 3, 0, 1, 2);

  QVBoxLayout *p_positionLayout = new QVBoxLayout;
  p_positionLayout->addWidget(pm_rememberPositionCheckBox);
  p_positionLayout->addLayout(p_positionGridLayout);

  QGroupBox *p_positionGroupBox = new QGroupBox;
  p_positionGroupBox->setTitle("Window position");
  p_positionGroupBox->setLayout(p_positionLayout);

  const bool rememberPosition = pm_rememberPositionCheckBox->isChecked();
  pm_initialPosXLineEdit->setEnabled(!rememberPosition);
  pm_initialPosYLineEdit->setEnabled(!rememberPosition);
  pm_applyPositionButton->setEnabled(!rememberPosition);
  pm_pickCurrentPositionButton->setEnabled(!rememberPosition);

  QLabel *p_keyLbl = new QLabel("Key for show/hide numpad: ");

  QComboBox *p_keysComboBox = new QComboBox;
  std::vector<ShowHideKey *> keys = pm_numpadManager->getShowHideKeysInfo();
  std::vector<ShowHideKey *>::iterator iter = keys.begin();
  while (iter != keys.end())
  {
    p_keysComboBox->addItem((*iter)->strRepresent);
    iter++;
  }
  ShowHideKey *p_currentKey = pm_numpadManager->getShowHideKey();
  int currentIndex = p_keysComboBox->findText(p_currentKey->strRepresent);
  p_keysComboBox->setCurrentIndex(currentIndex);
   
  QHBoxLayout *p_keyLayout = new QHBoxLayout;
  p_keyLayout->addWidget(p_keyLbl);
  p_keyLayout->addWidget(p_keysComboBox);
  p_keyLayout->addStretch(1);
  QGroupBox *p_keyGroupBox = new QGroupBox(this);
  p_keyGroupBox->setLayout(p_keyLayout);
  p_keyGroupBox->setEnabled(pm_numpadManager->keyboardHookIsSetGood());
  p_keyGroupBox->hide();
  
  QLabel *p_buttonsSizeLbl = new QLabel("Buttons size");
  QSlider *p_buttonsSizeSlider = new QSlider(Qt::Horizontal); 
  p_buttonsSizeSlider->setRange(pm_numpad->getMinButtonsSize(),
                                pm_numpad->getMaxButtonsSize());
  p_buttonsSizeSlider->setValue(initBtnSize); 
  p_buttonsSizeSlider->setSingleStep(1);
  p_buttonsSizeSlider->setPageStep(5);
  QHBoxLayout *p_buttonsSizeLayout = new QHBoxLayout;
  p_buttonsSizeLayout->addWidget(p_buttonsSizeLbl);
  p_buttonsSizeLayout->addWidget(p_buttonsSizeSlider);
  p_buttonsSizeLayout->addStretch(1);
      
  QLabel *p_spacingLbl = new QLabel("Spacing");
  QSlider *p_spacingSlider = new QSlider(Qt::Horizontal);  
  p_spacingSlider->setRange(pm_numpad->getMinSpacing(), pm_numpad->getMaxSpacing());
  p_spacingSlider->setValue(initSpacing);
  p_spacingSlider->setSingleStep(1);
  p_spacingSlider->setPageStep(5);
  QHBoxLayout *p_spacingLayout = new QHBoxLayout;
  p_spacingLayout->addWidget(p_spacingLbl);
  p_spacingLayout->addWidget(p_spacingSlider);
  p_spacingLayout->addStretch(1);
  
  QFontMetrics font = QFontMetrics(p_buttonsSizeLbl->font());
  int sizeGrpWidth = font.horizontalAdvance("Buttons size");
  p_buttonsSizeLbl->setFixedWidth(sizeGrpWidth);
  p_spacingLbl->setFixedWidth(sizeGrpWidth);
  p_buttonsSizeSlider->setFixedWidth(sizeGrpWidth * 2);
  p_spacingSlider->setFixedWidth(sizeGrpWidth * 2);

  QVBoxLayout *p_sizesLayout = new QVBoxLayout;
  p_sizesLayout->addLayout(p_buttonsSizeLayout);
  p_sizesLayout->addLayout(p_spacingLayout);
   
  QGroupBox *p_sizesGrpBox = new QGroupBox;
  p_sizesGrpBox->setLayout(p_sizesLayout);

  /*
  QCheckBox *p_altCodeLblModeCheckBox = new QCheckBox("Show alt-code");
  p_altCodeLblModeCheckBox->setChecked(pm_numpad->getAltCodeLblMode());

  QHBoxLayout *p_horAltCodeLblModeLayout = new QHBoxLayout;
  p_horAltCodeLblModeLayout->addWidget(p_altCodeLblModeCheckBox);
  p_horAltCodeLblModeLayout->addStretch(1);
  QGroupBox *p_altCodeLblModeGrpBox = new QGroupBox;
  p_altCodeLblModeGrpBox->setLayout(p_horAltCodeLblModeLayout);
*/

  QPushButton *p_confBtn = new QPushButton("Add/Remove buttons");  
  QPushButton *p_loadOtherConfBtn = new QPushButton("Load other configuration");
  QPushButton *p_openConfFileFolderBtn = new QPushButton("Open configuration files folder");
  QVBoxLayout *p_confLayout = new QVBoxLayout;
  p_confLayout->addWidget(p_confBtn);  
  p_confLayout->addWidget(p_loadOtherConfBtn);
  p_confLayout->addWidget(p_openConfFileFolderBtn);  

  QGroupBox *p_confGrpBox = new QGroupBox;
  p_confGrpBox->setLayout(p_confLayout);
  p_confGrpBox->setTitle("Buttons configuration");

  QPushButton *p_fontBtn = new QPushButton("Font");
  QVBoxLayout *p_fontLayout = new QVBoxLayout;
  p_fontLayout->addWidget(p_fontBtn);

  QGroupBox *p_fontGrpBox = new QGroupBox;
  p_fontGrpBox->setLayout(p_fontLayout);
  
  QPushButton *p_btnNotPressColorBtn = new QPushButton("Not pressed button");
  QPushButton *p_btnPressColorBtn = new QPushButton("Pressed button");
  QPushButton *p_backgroundColorBtn = new QPushButton("Background");
  QPushButton *p_textColorBtn = new QPushButton("Text");
  
  QVBoxLayout *p_vertColorLayout = new QVBoxLayout;
  p_vertColorLayout->addWidget(p_btnNotPressColorBtn);
  p_vertColorLayout->addWidget(p_btnPressColorBtn);
  p_vertColorLayout->addWidget(p_backgroundColorBtn);
  p_vertColorLayout->addWidget(p_textColorBtn);

  QGroupBox *p_colorGrpBox = new QGroupBox;
  p_colorGrpBox->setLayout(p_vertColorLayout);
  p_colorGrpBox->setTitle("Colors");   
  
  QVBoxLayout *p_mainLayout = new QVBoxLayout;
  p_mainLayout->addWidget(p_showGearGrpBox);
  p_mainLayout->addWidget(p_layoutBtnGrpBox);
  p_mainLayout->addWidget(p_autoRunGrpBox);
  p_mainLayout->addWidget(p_positionGroupBox);
  p_mainLayout->addWidget(p_sizesGrpBox);
 // p_mainLayout->addWidget(p_altCodeLblModeGrpBox);
  p_mainLayout->addWidget(p_confGrpBox);
  p_mainLayout->addWidget(p_fontGrpBox);
  p_mainLayout->addWidget(p_colorGrpBox);
  
  setLayout(p_mainLayout);
  
  connect(p_keysComboBox, SIGNAL(activated(const QString &)),
              SLOT(slot_keyComboBoxItemActivated(const QString &)));
  connect(p_buttonsSizeSlider, SIGNAL(valueChanged(int)), SLOT(slot_buttonsSizeChanged(int)));
  connect(p_spacingSlider, SIGNAL(valueChanged(int)), SLOT(slot_spacingChanged(int)));
  connect(p_btnNotPressColorBtn, SIGNAL(clicked()), SLOT(slot_btnNotPressColorBtnClicked()));
  connect(p_btnPressColorBtn, SIGNAL(clicked()), SLOT(slot_btnPressColorBtnClicked()));
  connect(p_backgroundColorBtn, SIGNAL(clicked()), SLOT(slot_backgroundColorBtnClicked()));
  connect(p_textColorBtn, SIGNAL(clicked()), SLOT(slot_textColorBtnClicked()));
  connect(p_fontBtn, SIGNAL(clicked()), SLOT(slot_fontBtnClicked()));
 // connect(p_altCodeLblModeCheckBox, SIGNAL(stateChanged(int)),
 //         SLOT(slot_altCodeLblModeStateChanged(int)));
  connect(p_autoRunCheckBox, SIGNAL(stateChanged(int)),
          SLOT(slot_autoRunStateChanged(int)));
  connect(p_buttonsSizeSlider, SIGNAL(sliderReleased()),
          SLOT(slot_buttonsSizeSliderReleased()));
  connect(p_spacingSlider, SIGNAL(sliderReleased()),
          SLOT(slot_spacingSliderReleased()));  
  connect(p_confBtn, SIGNAL(clicked()), SLOT(slot_confBtnClicked()));
  connect(p_showGearCheckBox, SIGNAL(stateChanged(int)),
          SLOT(slot_showGearStateChanged(int)));
  connect(p_showLayoutBtnCheckBox, SIGNAL(stateChanged(int)),
          SLOT(slot_showLayoutBtnStateChanged(int)));
  connect(p_openConfFileFolderBtn, SIGNAL(clicked()), SLOT(slot_openConfFileClicked()));
  connect(p_loadOtherConfBtn, SIGNAL(clicked()), SLOT(slot_loadOtherConfBtnClicked()));
  connect(pm_rememberPositionCheckBox, SIGNAL(stateChanged(int)),
          SLOT(slot_rememberPositionStateChanged(int)));
  connect(pm_initialPosXLineEdit, SIGNAL(editingFinished()),
          SLOT(slot_customPositionEditingFinished()));
  connect(pm_initialPosYLineEdit, SIGNAL(editingFinished()),
          SLOT(slot_customPositionEditingFinished()));
  connect(pm_pickCurrentPositionButton, SIGNAL(clicked()),
          SLOT(slot_pickCurrentPositionClicked()));
  connect(pm_applyPositionButton, SIGNAL(clicked()),
          SLOT(slot_applyCustomPositionClicked()));

  setFixedSize(sizeHint());

  HWND hwnd = (HWND)winId();
  LONG styles = GetWindowLong(hwnd, GWL_EXSTYLE);
  SetWindowLong(hwnd, GWL_EXSTYLE, styles | WS_EX_NOACTIVATE);
}

////////////////////////////////////////////////////////////////////////////////


void SettingsDialog::slot_keyComboBoxItemActivated(const QString & keyStr)
{
  if (pm_numpadManager->setShowHideKey(keyStr))
  {
      pm_numpadManager->writeKeyToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_buttonsSizeChanged(int size)
{  
  pm_numpad->setButtonsSize(size, true);  
}  

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_spacingChanged(int spacing)
{ 
  pm_numpad->setSpacing(spacing);   
}  

////////////////////////////////////////////////////////////////////////////////


void SettingsDialog::slot_btnNotPressColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getBtnNotPressColor(),
                                        this, "Not pressed buttons color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    pm_numpad->setBtnNotPressColor(color.name());   
    pm_numpadManager->writeBtnNotPressColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_btnPressColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getBtnPressColor(),
                                        this, "Pressed buttons color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    pm_numpad->setBtnPressColor(color.name());   
    pm_numpadManager->writeBtnPressColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_backgroundColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getBackgroundColor(),
                                        this, "Background color", 
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {    
    pm_numpad->setBackgroundColor(color.name());    
    pm_numpadManager->writeBackgroundColorToSettings();
  } 
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_textColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getBtnTextColor(),
                                        this, "Symbols color", 
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {    
    pm_numpad->setBtnTextColor(color.name());   
    pm_numpadManager->writeBtnTextColorToSettings();
  } 
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_fontBtnClicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, pm_numpad->getButtonsFont(),
                                      this, "Select Font");
    if (ok)
    {
        pm_numpad->setButtonsFont(font);       
        pm_numpadManager->writeFontToSettings();
    }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_altCodeLblModeStateChanged(int state)
{
    if (state == Qt::Checked)
    {
        pm_numpad->setAltCodeLblMode(true);
    }
    else if (state == Qt::Unchecked)
    {
        pm_numpad->setAltCodeLblMode(false);
    }
    pm_numpadManager->writeAltCodeLblMode();
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_autoRunStateChanged(int state)
{
    if (state == Qt::Checked)
    {
        pm_numpadManager->setAutoRun(true);
    }
    else if (state == Qt::Unchecked)
    {
        pm_numpadManager->setAutoRun(false);
    }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_buttonsSizeSliderReleased()
{
    pm_numpadManager->writeButtonsSizeToSettings();
    pm_numpadManager->writeFontToSettings();
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_spacingSliderReleased()
{
    pm_numpadManager->writeSpacingToSettings();
}

void SettingsDialog::setNumpad(Numpad *p_numpad)
{
    pm_numpad = p_numpad;    
}


void SettingsDialog::slot_confBtnClicked()
{
    pm_numpadManager->configure();
}


void SettingsDialog::slot_loadOtherConfBtnClicked()
{
    pm_numpadManager->loadOtherConfig();
}


void SettingsDialog::slot_showGearStateChanged(int state)
{
    pm_numpadManager->setMenuVisible(state);
}


void SettingsDialog::slot_showLayoutBtnStateChanged(int state)
{
    pm_numpadManager->setLayoutBtnVisible(state == Qt::Checked);
}


void SettingsDialog::slot_openConfFileClicked()
{
    pm_numpadManager->openConfFileFolder();
}


void SettingsDialog::slot_rememberPositionStateChanged(int state)
{
    const bool remember = (state == Qt::Checked);
    pm_numpadManager->writeRememberLastPositionToSettings(remember);

    if (remember && pm_numpad)
    {
        pm_numpadManager->writeNumpadPosition(pm_numpad->pos());
    }

    pm_initialPosXLineEdit->setEnabled(!remember);
    pm_initialPosYLineEdit->setEnabled(!remember);
    pm_applyPositionButton->setEnabled(!remember);
    pm_pickCurrentPositionButton->setEnabled(!remember);

    pm_numpadManager->applyInitialPosition(pm_numpadManager->readNumpadPosition());
}


void SettingsDialog::slot_customPositionEditingFinished()
{
    const QPoint fallback = pm_numpadManager->readInitialPositionFromSettings();
    const QPoint pos(parsePositionValue(pm_initialPosXLineEdit, fallback.x()),
                     parsePositionValue(pm_initialPosYLineEdit, fallback.y()));
    pm_numpadManager->writeInitialPositionToSettings(pos);

    if (!pm_rememberPositionCheckBox->isChecked())
    {
        pm_numpadManager->applyInitialPosition(pos);
    }
}


void SettingsDialog::slot_pickCurrentPositionClicked()
{
    if (!pm_numpad)
    {
        return;
    }

    const QPoint currentPos = pm_numpad->pos();
    pm_initialPosXLineEdit->setText(QString::number(currentPos.x()));
    pm_initialPosYLineEdit->setText(QString::number(currentPos.y()));

    slot_customPositionEditingFinished();
}


void SettingsDialog::slot_applyCustomPositionClicked()
{
    const QPoint fallback = pm_numpadManager->readInitialPositionFromSettings();
    const QPoint pos(parsePositionValue(pm_initialPosXLineEdit, fallback.x()),
                     parsePositionValue(pm_initialPosYLineEdit, fallback.y()));
    pm_numpadManager->applyInitialPosition(pos);
}

