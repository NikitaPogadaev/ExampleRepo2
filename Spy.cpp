#pragma once

#include <concepts>
#include <cmath>
#include <iostream>
#include <memory>




template<typename Logger>
concept MinReqLogger = std::invocable<Logger, unsigned int> && std::move_constructible<Logger>;



struct BaseWrapperCopyable {
    virtual BaseWrapperCopyable& operator = (const BaseWrapperCopyable& bw) = 0;
    virtual BaseWrapperCopyable& operator = (BaseWrapperCopyable&& bw) = 0;
    virtual BaseWrapperCopyable* Clone() = 0;
    virtual void Activate(std::size_t&) = 0;
    virtual ~BaseWrapperCopyable() {};
};

struct BaseWrapperMoveOnly {
    virtual BaseWrapperMoveOnly& operator = (BaseWrapperMoveOnly&& bw) = 0;
    virtual void Activate(std::size_t&) = 0;
    virtual ~BaseWrapperMoveOnly() {};
};


template<typename T>
struct WrapperMoveOnly : BaseWrapperMoveOnly {

    BaseWrapperMoveOnly& operator = (BaseWrapperMoveOnly&& bw) override{
        WrapperMoveOnly<T>&& ref = static_cast<WrapperMoveOnly<T>&&>(bw);
        foo_ = std::move(ref.foo_);
        return *this;
    }

    void Activate(std::size_t& count) override {
        foo_(count);
        count = 0;
    }

    template <typename U>
    WrapperMoveOnly(U&& foo) : foo_(std::forward<decltype(foo)>(foo)){}

    ~WrapperMoveOnly() override {}

    T foo_;
};

template<typename T> //requires (std::copyable<T>)
struct WrapperCopyable: BaseWrapperCopyable {

    BaseWrapperCopyable& operator = (const BaseWrapperCopyable& bw) override{
        foo_ = (static_cast<const WrapperCopyable<T>&>(bw)).foo_;
        return *this;
    }

    BaseWrapperCopyable& operator = (BaseWrapperCopyable&& bw) override{
        WrapperCopyable<T>&& ref = static_cast<WrapperCopyable<T>&&>(bw);
        foo_ = std::move(ref.foo_);
        return *this;
    }

    BaseWrapperCopyable* Clone() override {
        return new WrapperCopyable(foo_);
    }

    void Activate(std::size_t& count) override {
        foo_(count);
        count = 0;
    }

    WrapperCopyable(const T& foo) : foo_(foo){}

    template <typename U>
    WrapperCopyable(U&& foo) : foo_(std::forward<decltype(foo)>(foo)){}

    ~WrapperCopyable() override {}

    T foo_;
};

// Allocator is only used in bonus SBO tests,
// ignore if you don't need bonus points.
template <class T>
class Spy {
public:
  explicit Spy(T&& value) requires (std::move_constructible<T>)
  : value_(std::forward<decltype(value)>(value)) {}

  T& operator *(){
    return value_;
  }
  const T& operator *() const {
    value_;
  }

  friend class Proxy;

  struct Proxy {

    Proxy(Spy<T>* ptr) : arrowHelp_(ptr) {}

    T* operator -> (){
      if((arrowHelp_->logger_).get() != nullptr){
        ++(arrowHelp_->counter_);
      }
      return &(arrowHelp_->value_);
    }

    ~Proxy(){
      if((arrowHelp_->counter_) > 0){
        arrowHelp_->logger_->Activate(arrowHelp_->counter_);
      }
    }

    Spy<T>* arrowHelp_;
  };

  Proxy operator -> (){
    return Proxy(this);
  }

  Spy()
  requires (std::is_default_constructible_v<T> && (std::move_constructible<T>))
  : value_(), logger_(nullptr) {};

  /*
   * if needed (see task readme):
   *   default constructor
   *   copy and move construction
   *   copy and move assignment
   *   equality operators
   *   destructor
  */

  Spy (Spy&& spy) requires (std::move_constructible<T> && std::swappable<T> && std::assignable_from<T&, T>){
    value_ = std::move(spy.value_);
    logger_ = std::move(spy.logger_);
    counter_ = spy.counter_;
    spy.counter_ = 0;
  }

  Spy& operator = (Spy&& spy) requires (std::move_constructible<T> && std::assignable_from<T&, T> && std::assignable_from<T&, T>){
    ////////
    if(&spy == this) return *this;
    ////////
    value_ = std::move(spy.value_);
    logger_.reset(nullptr);
    logger_ = std::move(spy.logger_);
    counter_ = spy.counter_;
    spy.counter_ = 0;
    return *this;
  }

  Spy (const Spy& spy) requires (std::copy_constructible<T> && std::move_constructible<T> && std::assignable_from<T&, T>) : value_(spy.value_) {
    if(spy.logger_.get() != nullptr){
      logger_.reset(spy.logger_->Clone());
    }
    counter_ = 0;
  }

  void swap (Spy& a, Spy& b) requires (std::move_constructible<T> && std::assignable_from<T&, T> && std::swappable<T>){
    a.value_.swap(b.value_);
    a.logger_.swap(b.logger_);
    a.counter_ = 0;
    b.counter_ = 0;
  }

  Spy& operator = (const Spy& spy) requires (std::copy_constructible<T>){
    ////////
    if(&spy == this) return *this;
    ////////
    value_ = spy.value_;
    logger_.reset(nullptr);
    if(spy.logger_.get() != nullptr){
      logger_.reset(spy.logger_->Clone());
    }
    counter_ = 0;
    return *this;
  }

  bool operator == (const Spy<T>& spy) const requires (std::regular<T>) {
    return spy.value_ == value_;
  }

  ~Spy() = default;

  // Resets logger
  void setLogger(){
    logger_.reset(nullptr);
  }

  template <MinReqLogger Logger>
  requires ((!std::copy_constructible<T> || std::copy_constructible<Logger>))
  void setLogger(Logger&& logger){
    logger_.reset(nullptr);
      logger_.reset(
          std::conditional_t<std::copy_constructible<T>, BaseWrapperCopyable*, BaseWrapperMoveOnly*>
          (new std::conditional_t<std::copy_constructible<T>, WrapperCopyable<Logger>, WrapperMoveOnly<Logger>>(std::forward<decltype(logger)>(logger)))
      );
  }

private:
  std::size_t counter_ = 0;
  T value_;
  std::conditional_t<std::copy_constructible<T>, std::unique_ptr<BaseWrapperCopyable>, std::unique_ptr<BaseWrapperMoveOnly>> logger_ = nullptr;
};
