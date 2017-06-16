#include "mruby.h"
#include "mruby/variable.h"
#ifdef _WIN32
  #include <windows.h>
  #define SLEEP(ms)   Sleep((DWORD)(ms))
  #define USLEEP(us)  Sleep((DWORD)((us) / 1000))
  #define MILLIS()    GetTickCount()
  #define MICROS()    (GetTickCount() * 1000)
#else /* POSIX */
  #include <unistd.h>
  #include <time.h>
  #define SLEEP(ms)   usleep((useconds_t)(ms * 1000))
  #define USLEEP(us)  usleep((useconds_t)(us))

  #ifdef __MACH__ /* Mac */
    #include <mach/mach_time.h>
    #include <math.h>
    static mrb_int
    __gettime(uint64_t base_nsec)
    {
      mach_timebase_info_data_t tb;
      mach_timebase_info(&tb);
      double now = (double)mach_absolute_time();
      double nsec = now * (double)tb.numer / (double)tb.denom / (double)base_nsec;
      return (mrb_int)fmod(nsec, MRB_INT_MAX);
    }
    #define MILLIS()  (__gettime(1000000))
    #define MICROS()  (__gettime(1000))
  #else /* Linux */
    static mrb_int __millis(void)
    {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (mrb_int)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    }
    static mrb_int __micros(void)
    {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (mrb_int)(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
    }
    #define MILLIS()  (__millis())
    #define MICROS()  (__micros())
  #endif
#endif

/* PlatoSim::Machine.delay(ms) #=> nil */
static mrb_value
mrb_mach_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int ms;
  mrb_get_args(mrb, "i", &ms);
  SLEEP(ms);
  return mrb_nil_value();
}

/* PlatoSim::Machine.delay_us(us) #= nil */
static mrb_value
mrb_mach_delay_us(mrb_state *mrb, mrb_value self)
{
  mrb_int us;
  mrb_get_args(mrb, "i", &us);
  USLEEP(us);
  return mrb_nil_value();
}

/* PlatoSim::Machine.millis #=> Fixnum */
static mrb_value
mrb_mach_millis(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(MILLIS());
}

/* PlatoSim::Machine.micros #=> Fixnum */
static mrb_value
mrb_mach_micros(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(MICROS());
}

void
mrb_mruby_plato_machine_sim_gem_init(mrb_state *mrb)
{
  struct RClass *mach;
  struct RClass *sim = mrb_define_module(mrb, "PlatoSim");
  mach = mrb_define_class_under(mrb, sim, "Machine", mrb->object_class);

  mrb_define_class_method(mrb, mach, "delay",     mrb_mach_delay,     MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, mach, "delay_us",  mrb_mach_delay_us,  MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, mach, "millis",    mrb_mach_millis,    MRB_ARGS_NONE());
  mrb_define_class_method(mrb, mach, "micros",    mrb_mach_micros,    MRB_ARGS_NONE());
}

void
mrb_mruby_plato_machine_sim_gem_final(mrb_state *mrb)
{
}