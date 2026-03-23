#include <ruby/ruby.h>

/*
 * These symbols are provided by the stackprof shared object, which is
 * already loaded when this test helper is required.
 */
extern void stackprof_buffer_sample(void);
extern void (*stackprof_record_gc_samples_hook)(void);

static void
simulate_signal_handler(void)
{
    /* This is exactly what the signal handler does when it fires outside
     * of GC with stackprof_use_postponed_job == 0: it calls
     * stackprof_buffer_sample(), which invokes rb_profile_frames() and
     * overwrites _stackprof.frames_buffer with the current call stack. */
    stackprof_buffer_sample();
}

static VALUE
install_hook(VALUE self)
{
    stackprof_record_gc_samples_hook = simulate_signal_handler;
    return Qnil;
}

static VALUE
remove_hook(VALUE self)
{
    stackprof_record_gc_samples_hook = NULL;
    return Qnil;
}

void
Init_stackprof_test_helper(void)
{
    VALUE mod = rb_define_module("StackProfTestHelper");
    rb_define_singleton_method(mod, "install_gc_race_hook", install_hook, 0);
    rb_define_singleton_method(mod, "remove_gc_race_hook", remove_hook, 0);
}
