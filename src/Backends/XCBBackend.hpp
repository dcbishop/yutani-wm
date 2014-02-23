#ifndef XCBBACKEND_YUTANIWM_HPP_
#define XCBBACKEND_YUTANIWM_HPP_

#include <xcb/xcb.h>
#include <string>

namespace YutaniWM {

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
  void connectXCB();
  void disconnectXCB() noexcept;
  void checkConnectionError() const;
  void throwAndLogError(const std::string& error_message) const;

  void initialize();
  void setSetup();
  void setScreen();
  void initializeEvents();
  void initializeMouseButtons();
  void initializeMouseButton(const int button);

  xcb_connection_t* connection;
  int screen_id;
  xcb_screen_t* screen;
  const xcb_setup_t* setup;
};

} /* namespace YutaniWM */

#endif /* XCBBACKEND_YUTANIWM_HPP_ */
