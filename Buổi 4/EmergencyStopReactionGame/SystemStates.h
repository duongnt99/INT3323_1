#pragma once

#include <stdint.h>

enum class StateIndex
{
  Standby,
  Running,
  Warning,
  Succeeded,
  Failed
};

class State
{
public:
  virtual void enterState() = 0;
  virtual void loopState() = 0;
  virtual void exitState() = 0;
};

class Standby : public State
{
public:
  void exitState() override;
  void loopState() override;
  void enterState() override;
};

class Running : public State
{
private:
  static constexpr int MIN_DELAY = 3000;
  static constexpr int MAX_DELAY = 6000;

  unsigned long warningTime;
public:
  void exitState() override;
  void loopState() override;
  void enterState() override;
};

class Warning : public State
{
private:
  static constexpr int FLASH_DURATION = 80;
  static constexpr int FLASH_DELAY = 160;
  static constexpr int REACTION_TIMEOUT = 2000;

  unsigned long warningStart;
  unsigned long flashStart;
  bool isFlashing;

  void handleFlash();
  void handleButton();
public:
  void exitState() override;
  void loopState() override;
  void enterState() override;
};

class Result : public State
{
private:
  static constexpr int FLASH_DURATION = 500;
  static constexpr int FLASH_DELAY = 1000;
  static constexpr int NOTIFY_DURATION = 2000;

  unsigned long flashStart;
  unsigned long notifyStart;
  bool isFlashing;
protected:
  virtual uint8_t getLedPin() const = 0;
public:
  void exitState() override;
  void loopState() override;
  void enterState() override;
};

class Succeeded : public Result
{
protected:
  uint8_t getLedPin() const override;
};

class Failed : public Result
{
protected:
  uint8_t getLedPin() const override;
};