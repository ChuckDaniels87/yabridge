#pragma once

#include <vestige/aeffect.h>

#include <boost/process/child.hpp>

/**
 * This handles the communication between the Linux native VST plugin and the
 * Wine VST host. The functions below should be used as callback functions in an
 * `AEffect` object.
 */
class Bridge {
   public:
    /**
     * Initializes the Wine VST bridge. This sets up the STDIN/STDOUT streams
     * for event handling and a shared memory buffer for passing audio.
     *
     * TODO: Figure out whether shared memory gives us better throughput and/or
     *       lower overhead than using a Unix domain socket would.
     *
     * @throw std::runtime_error Thrown when the VST host could not be found, or
     *   if it could not locate and load a VST .dll file.
     */
    Bridge();

    // The four below functions are the handlers from the VST2 API. They are
    // called through proxy functions in `plugin.cpp`.

    /**
     * Handle an event sent by the VST host. Most of these opcodes will be
     * passed through to the winelib VST host.
     */
    intptr_t dispatch(AEffect* plugin,
                      int32_t opcode,
                      int32_t parameter,
                      intptr_t value,
                      void* result,
                      float option);
    void process(AEffect* plugin,
                 float** inputs,
                 float** outputs,
                 int32_t sample_frames);
    void set_parameter(AEffect* plugin, int32_t index, float value);
    float get_parameter(AEffect* plugin, int32_t index);

   private:
    boost::process::opstream vst_stdin;
    boost::process::ipstream vst_stdout;
    boost::process::child vst_host;
};