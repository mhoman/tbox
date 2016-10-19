/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the switch count
#define COUNT       (10000000)

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */ 
static tb_void_t tb_demo_coroutine_channel_send_func(tb_cpointer_t priv)
{
    // check
    tb_co_channel_ref_t channel = (tb_co_channel_ref_t)priv;

    // loop
    tb_size_t count = 10;
    while (count--)
    {
        // trace
        tb_trace_i("[coroutine: %p]: send: %lu", tb_coroutine_self(), count);

        // send data
        tb_co_channel_send(channel, (tb_cpointer_t)count);
 
        // trace
        tb_trace_i("[coroutine: %p]: send: %lu ok", tb_coroutine_self(), count);
    }
}
static tb_void_t tb_demo_coroutine_channel_recv_func(tb_cpointer_t priv)
{
    // check
    tb_co_channel_ref_t channel = (tb_co_channel_ref_t)priv;

    // loop
    tb_size_t count = 10;
    while (count--)
    {
        // trace
        tb_trace_i("[coroutine: %p]: recv: ..", tb_coroutine_self());

        // recv
        tb_size_t data = (tb_size_t)tb_co_channel_recv(channel);
 
        // trace
        tb_trace_i("[coroutine: %p]: recv: %lu ok", tb_coroutine_self(), data);
    }
}
static tb_void_t tb_demo_coroutine_channel_test(tb_size_t size)
{
    // trace
    tb_trace_i("test: %lu", size);

    // init scheduler
    tb_co_scheduler_ref_t scheduler = tb_co_scheduler_init();
    if (scheduler)
    {
        // init channel
        tb_co_channel_ref_t channel = tb_co_channel_init(size, tb_element_size());
        tb_assert(channel);

        // start coroutines
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_send_func, channel, 0);
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_send_func, channel, 0);
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_send_func, channel, 0);
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_recv_func, channel, 0);
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_recv_func, channel, 0);
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_recv_func, channel, 0);

        // run scheduler
        tb_co_scheduler_loop(scheduler);

        // exit channel 
        tb_co_channel_exit(channel);

        // exit scheduler
        tb_co_scheduler_exit(scheduler);
    }
}
static tb_void_t tb_demo_coroutine_channel_perf_func(tb_cpointer_t priv)
{
    // loop
    tb_size_t count = (tb_size_t)priv;
    while (count--)
    {
        // yield
        tb_coroutine_yield();
    }
}
static tb_void_t tb_demo_coroutine_channel_perf(tb_size_t size)
{
    // trace
    tb_trace_i("perf: %lu", size);

    // init scheduler
    tb_co_scheduler_ref_t scheduler = tb_co_scheduler_init();
    if (scheduler)
    {
        // start coroutine
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_perf_func, (tb_cpointer_t)(COUNT >> 1), 0);
        tb_coroutine_start(scheduler, tb_demo_coroutine_channel_perf_func, (tb_cpointer_t)(COUNT >> 1), 0);

        // init the start time
        tb_hong_t startime = tb_mclock();

        // run scheduler
        tb_co_scheduler_loop(scheduler);

        // computing time
        tb_hong_t duration = tb_mclock() - startime;

        // trace
        tb_trace_i("%d switches in %lld ms, %lld switches per second", COUNT, duration, (((tb_hong_t)1000 * COUNT) / duration));

        // exit scheduler
        tb_co_scheduler_exit(scheduler);
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */ 
tb_int_t tb_demo_coroutine_channel_main(tb_int_t argc, tb_char_t** argv)
{
//    tb_demo_coroutine_channel_test(0);
    tb_demo_coroutine_channel_test(1);
    tb_demo_coroutine_channel_test(5);

    tb_demo_coroutine_channel_perf(0);
    tb_demo_coroutine_channel_perf(1);
    tb_demo_coroutine_channel_perf(10);

    return 0;
}
