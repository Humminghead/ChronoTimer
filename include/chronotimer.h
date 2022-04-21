#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <utility>

namespace Estimate {

template <typename Tuple> struct Job {
  Tuple m_tuple;

  using Indices =
      typename std::index_sequence_for<typename std::tuple_size<Tuple>>;

  template <size_t... _Ind> auto Invoke(std::index_sequence<_Ind...>) noexcept {
    return std::invoke(std::get<_Ind>(std::move(m_tuple))...);
  }

  auto operator()() noexcept
      -> decltype(std::declval<Job &>().Invoke(Indices())) {
    return Invoke(Indices());
  }
};

class AbstractCall {
public:
  virtual ~AbstractCall() = default;
  virtual void run() = 0;

  auto &duration() { return m_duration; }

protected:
  std::chrono::high_resolution_clock::time_point m_time0;
  std::chrono::duration<double> m_duration;
};

template <typename Callable> struct CallWrapper : public AbstractCall {
  Callable m_function;

  CallWrapper(Callable &&func) : m_function(std::forward<Callable>(func)) {}

  void run() override {
    m_time0 = std::chrono::high_resolution_clock::now();
    m_function();
    m_duration = std::chrono::high_resolution_clock::now() - m_time0;
  }
};

class ChronoTimer {
  template <typename... Type>
  using DecayedTuple = std::tuple<typename std::decay<Type>::type...>;

  using m_callPointer = std::unique_ptr<AbstractCall>;

  template <typename _Callable>
  static m_callPointer MakeCallPointer(_Callable &&__f) {
    using _Impl = CallWrapper<_Callable>;
    return m_callPointer{new _Impl{std::forward<_Callable>(__f)}};
  }

  template <typename Callable, typename... Args>
  static Job<DecayedTuple<Callable, Args...>> MakeJob(Callable &&callable,
                                                      Args &&... args) {
    return {DecayedTuple<Callable, Args...>{std::forward<Callable>(callable),
                                            std::forward<Args>(args)...}};
  }

public:
  auto Run() { return m_statePointer->run(); }

  template <class Fmt = std::chrono::milliseconds> auto GetTime() const {
    return std::chrono::duration_cast<Fmt>(m_statePointer->duration()).count();
  }

  template <class Callable, class... Args>
  ChronoTimer(Callable &&callable, Args... args) {
    m_statePointer = MakeCallPointer(
        MakeJob(std::forward<Callable>(callable), std::forward<Args>(args)...));
  }

private:
  m_callPointer m_statePointer;
};
} // namespace Estimate
