#pragma once

#include <memory>
#include <vector>

#include "display/element.hpp"
#include "display/types.hpp"

namespace display
{

template<typename T = Element>
  requires(std::is_base_of_v<Element, T>)
class Layout : public Element
{
public:
  using ptr_t = std::unique_ptr<T>;

  explicit Layout(plc_t aplc)
      : Element(aplc)
  {
  }

  void resize(plc_t aplc) override
  {
    Element::resize(aplc);

    if (has_child()) {
      m_child->resize(aplc);
    }
  }

  void render() const override
  {
    if (has_child()) {
      m_child->render();
    }
  }

  void clear() const override
  {
    if (has_child()) {
      m_child->clear();
    }
  }

  void input(event& evnt) override
  {
    if (has_child()) {
      m_child->input(evnt);
    }
  }

  template<typename M = T, class... Args>
    requires(std::is_base_of_v<T, M>)
  M& set_child(Args&&... args)
  {
    clear();
    m_child = std::make_unique<M>(aplc(), std::forward<Args>(args)...);
    return get_child<M>();
  }

  template<typename M = T>
    requires(std::is_base_of_v<T, M>)
  const M& get_child() const
  {
    return *dynamic_cast<M*>(m_child.get());
  }

  template<typename M = T>
    requires(std::is_base_of_v<T, M>)
  M& get_child()
  {
    return *dynamic_cast<M*>(m_child.get());
  }

  bool has_child() const { return m_child != nullptr; }

private:
  ptr_t m_child;
};

template<typename T = Element>
  requires(std::is_base_of_v<Element, T>)
class LayoutMulti : public Element
{
public:
  using ptr_t = std::unique_ptr<T>;

  explicit LayoutMulti(plc_t aplc)
      : Element(aplc)
  {
  }

  void resize(plc_t aplc) override
  {
    Element::resize(aplc);

    for (std::size_t i = 0; i < size(); i++) {
      m_children[i]->resize(aplc);
    }
  }

  void render() const override
  {
    for (const auto& child : m_children) {
      child->render();
    }
  }

  void clear() const override
  {
    for (const auto& child : m_children) {
      child->clear();
    }
  }

  void input(event& evnt) override
  {
    for (auto& child : m_children) {
      child->input(evnt);
      if (evnt.type() == event::Type::NONE) {
        break;
      }
    }
  }

  template<typename M = T, class... Args>
    requires(std::is_base_of_v<T, M>)
  M& append(Args&&... args)
  {
    return append<M>(aplc(), std::forward<Args>(args)...);
  }

  template<typename M = T>
    requires(std::is_base_of_v<T, M>)
  const M& get(std::size_t idx) const
  {
    return *dynamic_cast<M*>(m_children[idx].get());
  }

  template<typename M = T>
    requires(std::is_base_of_v<T, M>)
  M& get(std::size_t idx)
  {
    return *dynamic_cast<M*>(m_children[idx].get());
  }

  std::size_t size() const { return m_children.size(); }

protected:
  template<typename M = T, class... Args>
    requires(std::is_base_of_v<T, M>)
  M& append(plc_t aplc, Args&&... args)
  {
    m_children.emplace_back(
        std::make_unique<M>(aplc, std::forward<Args>(args)...));
    return get<M>(m_children.size() - 1);
  }

private:
  std::vector<ptr_t> m_children;
};

}  // namespace display
