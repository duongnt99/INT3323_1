#pragma once

enum class StateIndex
{
  Ready,
  Stopped,
  Initiating
};

class State
{
public:
  virtual void enterState() = 0;
  virtual void loopState() = 0;
  virtual void exitState() = 0;
};

class Ready : public State
{
public:
  void loopState() override;
  void enterState() override;
  void exitState() override;
};

class Stopped : public State
{
private:
  const int FLASH_DURATION = 100;
  const int FLASH_DELAY = 350;

  unsigned long previousFlash;
  bool flashOn;

  void handleButton();
  void handleWarning();
public:
  void loopState() override;
  void enterState() override;
  void exitState() override;
};

class Initiating : public State
{
private:
  const int WARNING_DURATION = 3000;
  const int FLASH_DURATION = 500;
  const int FLASH_DELAY = 1000;

  unsigned long previousFlash;
  unsigned long warningTimeout;
  bool flashOn;

  void handleWarning();
  void handleButton();
public:
  void loopState() override;
  void enterState() override;
  void exitState() override;
};