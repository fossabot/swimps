#pragma once

#include <cassert>
#include <cstdarg>
#include <cstddef>

#include "swimps-io.h"

namespace swimps::log {

    enum class LogLevel {
        Fatal,   //! something bad *is* happening and it cannot be recovered from
        Error,   //! something bad *is* happening
        Warning, //! something bad *might* be happening
        Info,    //! something the user might want to know about is happening
        Debug    //! something the developer might want to know about is happening
    };

    //!
    //! \brief  Formats a message so that it's ready to be written to a log.
    //!
    //! \param[in]   logLevel  The kind of log message (error, info, etc).
    //! \param[in]   message   The message to be written.
    //! \param[out]  target    Where to write the formatted message.
    //!
    //! \returns  The number of bytes written to the target buffer.
    //!
    //! \note  If the target isn't big enough to fit the whole message,
    //!        it will be truncated.
    //!
    //! \note  The formatted message does not need to be null terminated.
    //!
    //! \note  The resulting message is not null terminated.
    //!
    //! \note  Message and target must not overlap.
    //!
    //! \note  This function is async signal safe.
    //!
    size_t log_message(
        const swimps::log::LogLevel logLevel,
        swimps::container::Span<const char> message,
        swimps::container::Span<char> target);

    //!
    //! \brief  Writes a message to all log targets, with the correct formatting.
    //!
    //! \param[in]  logLevel     The kind of log message (error, info, etc).
    //! \param[in]  message      The message to write.
    //! \param[in]  messageSize  The size of the message, in bytes.
    //!
    //! \returns  The number of bytes written to the log.
    //!
    //! \note  Messages must be less than 2048 bytes long *after* formatting.
    //!
    //! \note  This function is async signal safe.
    //!
    size_t write_to_log(
        const swimps::log::LogLevel logLevel,
        swimps::container::Span<const char> message);

    //!
    //! \brief  Formats a message and writes to all log targets
    //!
    //! \tparam     targetBufferSize  The size of the internal buffer to use for the formatted message.
    //!
    //! \param[in]  logLevel          The kind of log message (error, info, etc).
    //! \param[in]  formatBuffer      The format string.
    //! \param[in]  formatBufferSize  The format string size in bytes.
    //! \param[in]  ...               The args for the format buffer.
    //!
    //! \returns  The number of bytes written to the log.
    //!
    //! \note  This function is async signal safe.
    //!
    template <size_t targetBufferSize>
    size_t format_and_write_to_log(
        const swimps::log::LogLevel logLevel,
        swimps::container::Span<const char> format,
        ...) {

        char targetBuffer[targetBufferSize] = { };

        va_list varargs;
        va_start(varargs, format);

        const size_t bytesWritten = swimps::io::format_string_valist(
            format,
            targetBuffer,
            varargs
        );

        va_end(varargs);

        return swimps::log::write_to_log(
            logLevel,
            { targetBuffer, bytesWritten }
        );
    }
}
