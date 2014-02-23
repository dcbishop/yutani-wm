#ifndef XCBBACKEND_YUTANIWM_HPP_
#define XCBBACKEND_YUTANIWM_HPP_

#include <xcb/xcb.h>
#include <string>

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

 private:
  // RAII
  class XCBConnection {
    public:
      XCBConnection();
      ~XCBConnection();
      void checkConnectionError();
      operator xcb_connection_t*() const noexcept { return connection; }
      int getScreenId() const noexcept { return screen_id; }

    private:
      xcb_connection_t* connection;
      bool is_connected = false;
      int screen_id;
  };

  void initialize();
  void setSetup();
  void setScreen();
  void initializeEvents();
  void initializeMouseButtons();
  void initializeMouseButton(const int button);
  void initializeRoot();
  int getScreenId() { return connection.getScreenId(); }

  XCBConnection connection;
  xcb_screen_t* screen;
  const xcb_setup_t* setup;
  xcb_void_cookie_t cookie;
};

} /* namespace YutaniWM */

#endif /* XCBBACKEND_YUTANIWM_HPP_ */
