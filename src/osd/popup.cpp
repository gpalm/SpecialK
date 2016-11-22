/**
 * This file is part of Special K.
 *
 * Special K is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Special K is distributed in the hope that it will be useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Special K.
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/

#include <Windows.h>
#include <CEGUI/CEGUI.h>

class SK_PopupWindow {
friend class SK_PopupManager;

public:
  CEGUI::Window* getChild (const char* szName);
  operator CEGUI::Window* (void);

protected:
  SK_PopupWindow (const char* szLayout) {
    try {
      CEGUI::WindowManager& window_mgr =
        CEGUI::WindowManager::getDllSingleton ();

      window_ =
        window_mgr.loadLayoutFromFile (szLayout);
    } catch (...) {
    }
  }

  ~SK_PopupWindow (void) {
    if (window_ != nullptr) {
      try {
        CEGUI::WindowManager& window_mgr =
          CEGUI::WindowManager::getDllSingleton ();

        window_mgr.destroyWindow (window_);

        window_ = nullptr;
      } catch (...) {
      }
    }
  }

private:
  CEGUI::Window* window_;
};

CEGUI::Window*
SK_PopupWindow::getChild (const char* szName)
{
  if (window_ == nullptr)
    return nullptr;

  try {
    return window_->getChild (szName);
  } catch (...) {
  }

  return nullptr;
}

SK_PopupWindow::operator CEGUI::Window* (void)
{
  return window_;
}

class SK_PopupManager {
public:
  static SK_PopupManager* getInstance (void);

  SK_PopupWindow* createPopup      (const char* szLayout);
  void            destroyPopup     (SK_PopupWindow* popup);
  void            destroyAllPopups (void);

  void            drawAllPopups    (void);
  bool            isPopup          (SK_PopupWindow* popup);

  bool            OnDestroyPopup   (const CEGUI::EventArgs& e);

protected:
  SK_PopupManager (void);

private:
  static SK_PopupManager* __manager__;
  static CRITICAL_SECTION cs;

  std::map <SK_PopupWindow *, CEGUI::Window  *> popups_;
  std::map <CEGUI::Window  *, SK_PopupWindow *> popups_rev_;
};




CRITICAL_SECTION SK_PopupManager::cs;
SK_PopupManager* SK_PopupManager::__manager__ = nullptr;

SK_PopupManager*
SK_PopupManager::getInstance (void)
{
  if (__manager__ == nullptr) {
    InitializeCriticalSectionAndSpinCount (&cs, 1048576UL);
    __manager__ = new SK_PopupManager ();
  }

  return __manager__;
}

SK_PopupManager::SK_PopupManager (void)
{
}

bool
SK_PopupManager::isPopup (SK_PopupWindow* popup)
{
  EnterCriticalSection (&cs);

  if (popups_.count (popup)) {
    LeaveCriticalSection (&cs);
    return true;
  }

  LeaveCriticalSection (&cs);

  return false;
}

SK_PopupWindow*
SK_PopupManager::createPopup (const char* szLayout)
{
  EnterCriticalSection (&cs);

  SK_PopupWindow* popup =
    new SK_PopupWindow (szLayout);

  if (popup->window_ != nullptr) {
    popups_.insert     (std::make_pair (popup, popup->window_));
    popups_rev_.insert (std::make_pair (popup->window_, popup));

    popup->window_->subscribeEvent (
      "DestructStart",//CEGUI::Window::EventDestructionStarted,
        CEGUI::Event::Subscriber ( &SK_PopupManager::OnDestroyPopup,
                                     this
                                 )
    );

    LeaveCriticalSection (&cs);

    return popup;
  }

  LeaveCriticalSection (&cs);

  delete popup;

  return nullptr;
}

void
SK_PopupManager::destroyPopup (SK_PopupWindow* popup)
{
  EnterCriticalSection (&cs);

  if (isPopup (popup)) {
    delete popup;
  }

  LeaveCriticalSection (&cs);
}

void
SK_PopupManager::destroyAllPopups (void)
{
  EnterCriticalSection (&cs);

  auto it = popups_.begin ();

  while (it != popups_.end ()) {
    delete (it++)->first;
  }

  popups_.clear     ();
  popups_rev_.clear ();

  LeaveCriticalSection (&cs);
}

#include "../log.h"

bool
SK_PopupManager::OnDestroyPopup (const CEGUI::EventArgs& e)
{
  EnterCriticalSection (&cs);

  CEGUI::WindowEventArgs& win_event =
    (CEGUI::WindowEventArgs &)e;

  if (popups_rev_.count (win_event.window)) {
    popups_.erase     (popups_rev_ [win_event.window]);
    popups_rev_.erase (win_event.window);
  }

  LeaveCriticalSection (&cs);

  return true;
}