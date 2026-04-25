#pragma once

#include <iostream>
#include <tuple>

#include "entity/entity.h"

#include "query/query.h"
#include "registry/registry.h"

// helper
namespace ddknd::view::detail
{
    template <typename List>
    struct HasAll;

    template <typename... Ts>
    struct HasAll<query::TypeList<Ts...>>
    {
        using Registry = registry::Registry;
        using Entity = entity::Entity;
        static bool check(Registry* regs, Entity e)
        {
            return (regs->template HasComponent<Ts>(e) && ...);
        }
    };

    template <typename List>
    struct HasNone;

    template <typename... Ts>
    struct HasNone<query::TypeList<Ts...>>
    {
        using Registry = registry::Registry;
        using Entity = entity::Entity;
        static bool check(Registry* regs, Entity e)
        {
            return (!regs->template HasComponent<Ts>(e) && ...);
        }
    };

    template <typename S, typename R, bool IncludeEntity>
    struct DerefTuple;

    template <typename S, typename... R>
    struct DerefTuple<S, query::TypeList<R...>, false>
    {
        using type = std::tuple<S&, R&...>;

        template <typename Registry, typename Storage>
        static type make(Registry* regs, Storage& selectedStorage, std::size_t idx)
        {
            auto e = selectedStorage.EntityAt(idx);

            return type{selectedStorage.ComponentAt(idx), regs->template GetComponent<R>(e)...};
        }
    };

    template <typename S, typename... R>
    struct DerefTuple<S, query::TypeList<R...>, true>
    {
        using Entity = entity::Entity;
        using type = std::tuple<Entity, S&, R&...>;

        template <typename Registry, typename Storage>
        static type make(Registry* regs, Storage& selectedStorage, std::size_t idx)
        {
            auto e = selectedStorage.EntityAt(idx);

            return type{e, selectedStorage.ComponentAt(idx), regs->template GetComponent<R>(e)...};
        }
    };

    

} // namespace ddknd::view::detail

namespace ddknd::view
{
    struct ViewIterator
    {
        using Registry = ::ddknd::registry::Registry;
        Registry* regs_;
        std::size_t idx;
        bool operator!=(const ViewIterator& o) const;
        ViewIterator& operator++();
        auto operator*() const;

      private:
        void advance_to_avoid();
    };

    template <typename Query, bool IncludeEntity = false>
    class View
    {
      public:
        using Registry = ::ddknd::registry::Registry;

        using selected = Query::selected;
        using requiredList = Query::requiredList;
        using excludedList = Query::excludedList;

        View(Query q, Registry* regs) : q(q), regs_(regs) {};

        void Print()
        {
            std::cerr << "selected=" << typeid(selected).name() << "\n";
            std::cerr << "required=" << typeid(requiredList).name() << "\n";
            std::cerr << "excluded=" << typeid(excludedList).name() << "\n";
        }

        struct Iterator
        {
            using Registry = ::ddknd::registry::Registry;
            using Entity = ::ddknd::entity::Entity;
            Registry* regs_;
            std::size_t idx;
            bool operator!=(const Iterator& o) const
            {
                return idx != o.idx;
            }
            Iterator& operator++()
            {
                idx++;
                advance_to_valid();
                return *this;
            }

            auto operator*() const
            {
                auto& st = regs_->AssureStorage<selected>();
                return detail::DerefTuple<
                    selected,
                    requiredList,
                    IncludeEntity
                >::make(regs_, st, idx);
            }

            void advance_to_valid()
            {
                auto& st = regs_->AssureStorage<selected>();

                while (idx < st.Size())
                {
                    auto e = st.EntityAt(idx);

                    if (satisfies(e))
                    {
                        return;
                    }

                    ++idx;
                }
            }

          private:
            bool satisfies(Entity e) const
            {
                return detail::HasAll<requiredList>::check(regs_, e) && detail::HasNone<excludedList>::check(regs_, e);
            }
        };

        auto withEntity() const
        {
            return View<Query, true>(q, regs_);
        }

        Iterator begin()
        {
            Iterator it{regs_, 0};
            it.advance_to_valid();
            return it;
        };
        Iterator end()
        {
            return Iterator{regs_, selected_size()};
        }

      private:
        Registry* regs_;
        Query q;

        std::size_t selected_size() const
        {
            return regs_->AssureStorage<selected>().Size();
        }
    };

} // namespace ddknd::view

// to define
namespace ddknd::registry
{
    template <typename Query>
    auto Registry::view(Query q)
    {
        return view::View<Query>{q, this};
    }
} // namespace ddknd::registry