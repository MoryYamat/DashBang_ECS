#pragma once

#include <iostream>
#include <tuple>

#include "ddknd/ecs/entity/entity.h"

#include "ddknd/ecs/query/query.h"
#include "ddknd/ecs/registry/registry.h"
#include "ddknd/ecs/storage/storage_fwd.h"

// helper
namespace ddknd::view::detail
{
    template<typename List>
    struct StoragePointerTuple;

    template<typename... R>
    struct StoragePointerTuple<query::TypeList<R...>>
    {
        using Registry = registry::Registry;
        using type = std::tuple<storage::Storage<R>*...>;

        template<typename Registry>
        static type make(Registry* regs)
        {
            return type{&regs->template AssureStorage<R>()...};
        }
    };

    template <typename List>
    struct HasAll;

    template <typename... Ts>
    struct HasAll<query::TypeList<Ts...>>
    {
        using Registry = registry::Registry;
        using Entity = entity::Entity;
        template<typename Tuple>
        static bool check(const Tuple& storages, Entity e)
        {
            return std::apply(
                [&](auto*... st)
                {
                    return(st->Has(e) && ...);
                },
                storages
            );
        }
    };

    template <typename List>
    struct HasNone;

    template <typename... Ts>
    struct HasNone<query::TypeList<Ts...>>
    {
        using Registry = registry::Registry;
        using Entity = entity::Entity;
        template<typename Tuple>
        static bool check(const Tuple& storages, Entity e)
        {
            return std::apply(
                [&](auto*... st)
                {
                    return(!st->Has(e) && ...);
                },
                storages
            );
        }
    };

    template <typename S, typename R, bool IncludeEntity>
    struct DerefTuple;

    template <typename S, typename... R>
    struct DerefTuple<S, query::TypeList<R...>, false>
    {
        using type = std::tuple<S&, R&...>;

        // template <typename Registry, typename Storage>
        // static type make(Registry* regs, Storage& selectedStorage, std::size_t idx)
        // {
        //     auto e = selectedStorage.EntityAt(idx);

        //     return type{selectedStorage.ComponentAt(idx), regs->template GetComponent<R>(e)...};
        // }

        template<typename Storage, typename RequiredStorages>
        static type make(Storage& selectedStorage, const RequiredStorages& requiredStorages, std::size_t idx)
        {
            auto e = selectedStorage.EntityAt(idx);
            return std::apply(
                [&](auto*... requiredStorage) -> type{
                    return type{selectedStorage.ComponentAt(idx), *requiredStorage->Get(e)...};
                },
                requiredStorages
            );
        }
    };

    template <typename S, typename... R>
    struct DerefTuple<S, query::TypeList<R...>, true>
    {
        using Entity = entity::Entity;
        using type = std::tuple<Entity, S&, R&...>;

        template<typename Storage, typename RequiredStorages>
        static type make(Storage& selectedStorage, const RequiredStorages& requiredStorages, std::size_t idx)
        {
            auto e = selectedStorage.EntityAt(idx);
            return std::apply(
                [&](auto*... requiredStorage) -> type{
                    return type{e, selectedStorage.ComponentAt(idx), *requiredStorage->Get(e)...};
                },
                requiredStorages
            );
        }
    };

    

} // namespace ddknd::view::detail

namespace ddknd::view
{
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
            using SelectedStorage = typename ::ddknd::storage::Storage<selected>;
            using RequiredStorage = typename detail::StoragePointerTuple<requiredList>::type;
            using ExcludedStorage = typename detail::StoragePointerTuple<excludedList>::type;
            
            Registry* regs_ = nullptr;
            SelectedStorage* selected_ = nullptr;
            RequiredStorage required_{};
            ExcludedStorage excluded_{};

            std::size_t idx;

            Iterator(Registry* regs, std::size_t idx)
                : regs_(regs), 
                selected_(&regs->template AssureStorage<selected>()), 
                required_(detail::StoragePointerTuple<requiredList>::make(regs)),
                excluded_(detail::StoragePointerTuple<excludedList>::make(regs)),
                idx(idx) {}

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
                // return detail::DerefTuple<
                //     selected,
                //     requiredList,
                //     IncludeEntity
                // >::make(regs_, *selected_, idx);
                return detail::DerefTuple<
                        selected,
                        requiredList,
                        IncludeEntity
                        >::make(*selected_, required_, idx);
            }

            void advance_to_valid()
            {

                while (idx < selected_->Size())
                {
                    auto e = selected_->EntityAt(idx);

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
                return detail::HasAll<requiredList>::check(required_, e) && detail::HasNone<excludedList>::check(excluded_, e);
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
            return regs_->template AssureStorage<selected>().Size();
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