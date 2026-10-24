#pragma once

enum class StateIndex
{
  Normal,
  Watch,
  HotDry,
  MQAlert,
  DhtError
};

class State
{
public:
  virtual void enterState() = 0;
  virtual void loopState() = 0;
  virtual void exitState() = 0;
};

class Normal : public State
{
public:
  void enterState() override;
  void loopState() override;
  void exitState() override;
};

class DhtError : public State
{
public:
  void enterState() override;
  void loopState() override;
  void exitState() override;
};

class Warning : public State
{
private:
  unsigned long lastWarning;
  bool isWarning;
protected:
  int warningDuration;
  int warningDelay;
  int ledPin;
  bool buzzerWarn;
  bool ledFlash;

  Warning(int _warningDuration, int _warningDelay, int _ledPin, bool _buzzerWarn, bool _ledFlash);
public:
  void enterState() override;
  void loopState() override;
  void exitState() override;
};

class Watch : public Warning
{
public:
  Watch();
};

class HotDry : public Warning
{
public:
  HotDry();
};

class MQAlert : public Warning
{
public:
  MQAlert();
};