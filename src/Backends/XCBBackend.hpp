#ifndef XCBBACKEND_YUTANIWM_HPP_
#define XCBBACKEND_YUTANIWM_HPP_

#include <xcb/xcb.h>
#include <string>

#include "Names.hpp"

namespace YutaniWM {

void throwAndLogError(const std::string& error_message);

class XCBBackend {
 public:
  // No accidental casting
  explicit XCBBackend();
  ~XCBBackend();

  // Delete copy constructor
  XCBBackend(const XCBBackend&) = delete;

  // Delete assignment construtor
  XCBBackend& operator=(const XCBBackend&) = delete;

  // Delete move constructor
  XCBBackend(XCBBackend&&) = delete;

  // Delete move assignemnt
  XCBBackend& operator=(XCBBackend&&) = delete;

  void eventLoop();

 private:
  // RAII
  class XCBConnection {
    public:
      XCBConnection();
      ~XCBConnection() noexcept;
      bool checkConnectionError() const noexcept;
      operator xcb_connection_t*() const noexcept { return connection; }
      int getScreenId() const noexcept { return screen_id; }

    private:
      xcb_connection_t* connection = nullptr;
      bool is_connected = false;
      int screen_id = -1;
  };

  void initialize();
  void setSetup();
  void setScreen();
  void initializeEvents();
  void initializeMouseButtons();
  void initializeMouseButton(const int button);
  void initializeRoot();
  int getScreenId() { return connection.getScreenId(); }
  void handleEvent(const xcb_generic_event_t *ev);
  void handleEvents();

  XCBConnection connection;
  xcb_screen_t* screen = nullptr;
  const xcb_setup_t* setup = nullptr;
  xcb_void_cookie_t cookie;
};

} /* namespace YutaniWM */

#endif /* XCBBACKEND_YUTANIWM_HPP_ */
