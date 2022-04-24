#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <utility>

namespace {
template <typename Tuple> struct TupledCall {
  Tuple m_tuple;

  using Indices =
      typename std::index_sequence_for<typename std::tuple_size<Tuple>>;

  template <size_t... _Ind> auto Invoke(std::index_sequence<_Ind...>) noexcept {
    return std::invoke(std::get<_Ind>(std::move(m_tuple))...);
  }

  auto operator()() noexcept
      -> decltype(std::declval<TupledCall &>().Invoke(Indices())) {
    return Invoke(Indices());
  }
};

class TimerBase {
public:
  virtual ~TimerBase() = default;

  auto &duration() { return m_duration; }

protected:
  std::chrono::high_resolution_clock::time_point m_time0;
  std::chrono::duration<double> m_duration;
};

template <class RetType> struct CallSelector : public TimerBase {
  template <class Call> auto Run(Call &&callPtr) -> RetType {
    m_time0 = std::chrono::high_resolution_clock::now();
    return [&] {
      auto result = callPtr();
      m_duration = std::chrono::high_resolution_clock::now() - m_time0;
      return result;
    }();
  }
};

template <> struct CallSelector<void> : public TimerBase {
  template <class Call> void Run(Call &&callPtr) {
    m_time0 = std::chrono::high_resolution_clock::now();
    callPtr();
    m_duration = std::chrono::high_resolution_clock::now() - m_time0;
  }
};

template <typename Callable> class RunHelper {
public:
  RunHelper(Callable &&func)
      : m_function(std::forward<Callable>(func)), m_selector{} {}

  auto Run() { return m_selector.Run(m_function); }
  auto Duration() { return m_selector.duration(); }

private:
  Callable m_function;
  CallSelector<decltype(m_function())> m_selector;
};

template <class Stream, class InfoGetter> struct LazyPrint {
  explicit LazyPrint(Stream &out, InfoGetter &&getter)
      : m_stream{out}, m_timeGetter{std::forward<InfoGetter>(getter)} {}
  ~LazyPrint() { m_stream << m_timeGetter() << '\n'; }

  Stream &m_stream;
  InfoGetter m_timeGetter;
};
}; // namespace

namespace Estimate {

using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds = std::chrono::nanoseconds;

template <class Callable, class... Args> class ChronoTimer {
  template <typename... Type>
  using DecayedTuple = std::tuple<typename std::decay<Type>::type...>;
  using WrappedCall = TupledCall<DecayedTuple<Callable, Args...>>;
  using CallPointer = std::unique_ptr<RunHelper<WrappedCall>>;

  static CallPointer MakeCallPointer(WrappedCall &&callable) {
    using Impl = RunHelper<WrappedCall>;
    return std::unique_ptr<Impl>{new Impl(std::forward<WrappedCall>(callable))};
  }

  static WrappedCall WrapCall(Callable &&callable, Args &&...args) {
    return {DecayedTuple<Callable, Args...>{std::forward<Callable>(callable),
                                            std::forward<Args>(args)...}};
  }

public:
  auto Run() { return m_statePointer->Run(); }

  template <class Out, class Fmt = Microseconds> auto RunAndPrint(Out &stream) {
    LazyPrint lazyPrint(stream, [&] { return GetTime<Fmt>(); });
    return m_statePointer->Run();
  }

  template <class Fmt = Microseconds> auto GetTime() const {
    return std::chrono::duration_cast<Fmt>(m_statePointer->Duration()).count();
  }

  ChronoTimer(Callable &&callable, Args... args) {
    m_statePointer = MakeCallPointer(WrapCall(std::forward<Callable>(callable),
                                              std::forward<Args>(args)...));
  }

private:
  CallPointer m_statePointer;
};
} // namespace Estimate
