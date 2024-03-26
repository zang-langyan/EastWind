#ifndef EW_TIMESTEP_H
#define EW_TIMESTEP_H


namespace EastWind {

class Timestep
{
public:
  Timestep(float time = 0.f)
    : m_time(time)
  {
  }

  operator float() const { return m_time; }

  float GetSeconds() const { return m_time; }
  float GetMilliSeconds() const { return m_time * 1000.f; }

private:
  float m_time;
};

}

#endif // !EW_TIMESTEP_H
