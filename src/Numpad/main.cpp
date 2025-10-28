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


#include "NumpadManager.h"
#include "SingleInstanceGuard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  SingleInstanceGuard instanceGuard(QStringLiteral("VirtualNumpadInstance"));
  if (!instanceGuard.tryToRun())
  {
    instanceGuard.notifyExistingInstance();
    return 0;
  }

  NumpadManager numpadManager;
  QObject::connect(&instanceGuard, &SingleInstanceGuard::activationRequested,
                   &numpadManager, &NumpadManager::bringToForeground);
  return app.exec();
}
