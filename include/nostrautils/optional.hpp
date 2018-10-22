/**
\file nostrautils/optional.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides a data structure that may or may not hold a value of a certain type.

\details
This data structure has two main uses:
<ul>
    <li>
        Delayed initialization: Allocate memory for an object without calling the constructor of that object.
    </li>
    <li>
        Returning invalid objects: Return an object that is not initialized.
    </li>
</ul>

\par_example
\code{.cpp}
class Test
{
public:
    Test()
    {
        std::cout << "Test::Test()\n";
    }
};

nou::Optional<Test> opt; // the wrapped object is not initialized yet

std::cout << "opt.isValid(): " << opt.isValid() << "\n";

opt = Test(); // the wrapped object is now initialized

std::cout << "opt.isValid(): " << opt.isValid() << "\n";
\endcode

For a more detailed example, see \link optional.ex.cpp here\endlink.
*/

/**
\example optional.ex.cpp

An example that demonstrates the usage of the optional component.
*/

#ifndef NOU_OPTIONAL_HPP
#define NOU_OPTIONAL_HPP

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

#ifndef NOU_UTIL_HPP
#    include "nostrautils/util.hpp"
#endif

namespace nou
{
    template<typename T>
    class Optional;

    ///\internal
    namespace internal
    {
        struct InvalidOpt final
        {
            InvalidOpt()                   = default;
            InvalidOpt(const InvalidOpt &) = delete; // avoid copy; to save performance

            // empty dummy type
        };

        constexpr InvalidOpt INVALID_OPT_INSTANCE = InvalidOpt{};

        /**
        \tparam T
        The type of the wrapped object.

        \tparam TRIV_DEST
        If \ilc{T} is trivially destructible. For this class, this is always \ilc{true} (there is a
        specialization for \ilc{false}).

        \brief
        The base class of \ilc{nou::Optional} for all types that are trivially destructible.
        */
        template<typename T, boolean TRIV_DEST>
        class OptionalStorage
        {
        protected:
            /**
            \brief
            The type of the wrapped object.
            */
            using Type = T;

            struct Empty
            {
                // empty dummy type
            };

            union
            {
                /**
                \brief
                The wrapped object. This union member is active if the \ilc{nou::Optional} instance is valid.
                */
                Type m_data;

                /**
                \brief
                The wrapped object. This union member is active if the \ilc{nou::Optional} instance is not
                valid.
                */
                Empty m_dummy;
            };

            /**
            \brief
            The valid state.

            \details
            \ilc{true} = valid, \ilc{false} = invalid.
            */
            boolean m_isValid;

            /**
            \brief
            A dummy type used by some constructors.
            */
            struct DummyValid
            {
                // empty dummy type
            };

            /**
            \brief
            A dummy type used by some constructors.
            */
            struct DummyInvalid
            {
                // empty dummy type
            };

            /**
            \tparam OT
            The type of the wrapped object of \ilc{other}.

            \param dummy
            A dummy parameter. Will not be used.

            \param other
            The optional to copy the data from.

            \brief
            Constructs a new instance by copying the wrapped object of \ilc{other}.

            \details
            If this constructor is called, \ilc{other} needs to be valid.
            */
            template<typename OT>
            constexpr OptionalStorage(const DummyValid &dummy, const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value);

            /**
            \tparam OT
            The type of the wrapped object of \ilc{other}.

            \param dummy
            A dummy parameter. Will not be used.

            \param other
            Will not be used.

            \brief
            Constructs a new instance in an invalid state.
            */
            template<typename OT>
            constexpr OptionalStorage(const DummyInvalid &dummy, const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value);

        protected:
            /**
            \brief
            Constructs a new instance in an invalid state.
            */
            constexpr OptionalStorage() noexcept;

            /**
            \tparam ARGS
            The types of the parameters that will be used to construct the wrapped object. As a consequence,
            an instance of \ilc{Type} needs to be constructible from instances of these types.

            \param args
            The arguments that will be used to construct the wrapped object. The values will be forwarded to
            the constructor of \ilc{Type}.

            \brief
            Constructs a new instance in a valid state with a wrapped object that was constructed from the
            passed parameters.
            */
            template<typename... ARGS, typename = EnableIfType<IsConstructible<Type, ARGS...>::value>>
            constexpr OptionalStorage(ARGS &&... args) noexcept(IsNothrowConstructible<Type, ARGS...>::value);

            /**
            \tparam OT
            The type of the object that the passed instance wraps around. \ilc{Type} must be constructible
            from this type.

            \param other
            The instance that holds the object that will be copied into the new instance.

            \brief
            Copies the object of \ilc{other} into the one that is being constructed.

            \details
            If \ilc{other} is not valid, the instance under construction will be initialized in an invalid
            state. The wrapped object will not be copied in any way; such copy will only occur if \ilc{other}
            is valid.
            */
            template<typename OT>
            constexpr OptionalStorage(const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value);

            /**
            \brief
            Does nothing (would destroy the wrapped object, but the destructors of the wrapped objects in this
            class are trivial).
            */
            constexpr void destroy() noexcept
            {}

            ~OptionalStorage() noexcept = default;
        };

        /**
        \tparam T
        The type of the wrapped object.

        \brief
        The base class of \ilc{nou::Optional} for all types that are not trivially destructible.
        */
        template<typename T>
        class OptionalStorage<T, false>
        {
        protected:
            /**
            \brief
            The type of the wrapped object.
            */
            using Type = T;

            struct Empty
            {
                // empty dummy type
            };

            union
            {
                /**
                \brief
                The wrapped object. This union member is active if the \ilc{nou::Optional} instance is valid.
                */
                Type m_data;

                /**
                \brief
                The wrapped object. This union member is active if the \ilc{nou::Optional} instance is not
                valid.
                */
                Empty m_dummy;
            };

            /**
            \brief
            The valid state.

            \details
            \ilc{true} = valid, \ilc{false} = invalid.
            */
            boolean m_isValid;

            /**
            \brief
            A dummy type used by some constructors.
            */
            struct DummyValid
            {
                // empty dummy type
            };

            /**
            \brief
            A dummy type used by some constructors.
            */
            struct DummyInvalid
            {
                // empty dummy type
            };

            /**
            \tparam OT
            The type of the wrapped object of \ilc{other}.

            \param dummy
            A dummy parameter. Will not be used.

            \param other
            The optional to copy the data from.

            \brief
            Constructs a new instance by copying the wrapped object of \ilc{other}.

            \details
            If this constructor is called, \ilc{other} needs to be valid.
            */
            template<typename OT>
            constexpr OptionalStorage(const DummyValid &dummy, const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value);

            /**
            \tparam OT
            The type of the wrapped object of \ilc{other}.

            \param dummy
            A dummy parameter. Will not be used.

            \param other
            Will not be used.

            \brief
            Constructs a new instance in an invalid state.
            */
            template<typename OT>
            constexpr OptionalStorage(const DummyInvalid &dummy, const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value);

        protected:
            /**
            \brief
            Constructs a new instance in an invalid state.
            */
            inline OptionalStorage() noexcept;

            /**
            \tparam ARGS
            The types of the parameters that will be used to construct the wrapped object. As a consequence,
            an instance of \ilc{Type} needs to be constructible from instances of these types.

            \param args
            The arguments that will be used to construct the wrapped object. The values will be forwarded to
            the constructor of \ilc{Type}.

            \brief
            Constructs a new instance in a valid state with a wrapped object that was constructed from the
            passed parameters.
            */
            template<typename... ARGS, typename = EnableIfType<IsConstructible<Type, ARGS...>::value>>
            inline OptionalStorage(ARGS &&... args) noexcept(IsNothrowConstructible<Type, ARGS...>::value);

            /**
            \tparam OT
            The type of the object that the passed instance wraps around. \ilc{Type} must be constructible
            from this type.

            \param other
            The instance that holds the object that will be copied into the new instance.

            \brief
            Copies the object of \ilc{other} into the one that is being constructed.

            \details
            If \ilc{other} is not valid, the instance under construction will be initialized in an invalid
            state. The wrapped object will not be copied in any way; such copy will only occur if \ilc{other}
            is valid.
            */
            template<typename OT>
            inline OptionalStorage(const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value);

            /**
            \brief
            If it is valid, calls the destructor of the wrapped object.
            */
            void destroy() noexcept(IsNothrowDestructible<Type>::value);

            /**
            \brief
            Calls \ilc{destroy()}.
            */
            ~OptionalStorage() noexcept(IsNothrowDestructible<Type>::value);
        };

    } // namespace internal
      ///\endinternal

    template<typename T>
    class Optional final : private internal::OptionalStorage<T, IsTriviallyDestructible<T>::value>
    {
        static_assert(!(IsReference<T>::value), "T may not be a reference.");
        static_assert(!(AreSame<T, internal::InvalidOpt>::value));

    private:
        /**
        \brief
        The type of the base class. For easier access to the type.
        */
        using Base = internal::OptionalStorage<T, IsTriviallyDestructible<T>::value>;

    public:
        /**
        \brief
        The type of the wrapped object.
        */
        using Type = typename Base::Type;

        /**
        \brief
        Constructs a new instance in an invalid state.
        */
        constexpr Optional() noexcept;

        /**
        \tparam ARGS
        The types of the parameters that will be used to construct the wrapped object. As a consequence, an
        instance of \ilc{Type} needs to be constructible from instances of these types.

        \param args
        The arguments that will be used to construct the wrapped object. The values will be forwarded to the
        constructor of \ilc{Type}.

        \brief
        Constructs a new instance in a valid state with a wrapped object that was constructed from the passed
        parameters.
        */
        template<typename... ARGS, typename = EnableIfType<IsConstructible<T, ARGS...>::value>>
        constexpr Optional(ARGS &&... args) noexcept(IsNothrowConstructible<Type, ARGS...>::value);

        /**
        \tparam OT
        The type of the object that the passed instance wraps around. \ilc{Type} must be constructible from
        this type.

        \param other
        The instance that holds the object that will be copied into the new instance.

        \brief
        Copies the object of \ilc{other} into the one that is being constructed.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be copied in any way; such copy will only occur if \ilc{other} is valid.
        */
        template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
        constexpr Optional(const Optional<OT> &other) noexcept(
            IsNothrowConstructible<Type, const OT &>::value);

        /**
        \tparam OT
        The type of the object that the passed instance wraps around. \ilc{Type} must be constructible from
        this type.

        \param other
        The instance that holds the object that will be moved into the new instance.

        \brief
        Moved the object of \ilc{other} into the one that is being constructed.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be moved in any way; such move operation will only occur if \ilc{other} is
        valid.

        Also, even if the wrapped object of \ilc{other} will be moved, \ilc{other} will stay valid.
        */
        template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
        inline Optional(Optional<OT> &&other) noexcept(IsNothrowConstructible<Type, OT &&>::value);

        /**
        \param invalidOpt
        A dummy parameter. Will not be used.

        \brief
        Constructs a new instance in an invalid state.

        \details
        This constructor should never be used explicitly, its sole purpose is to be used with
        \ilc{nou::invalidOpt()}.
        */
        constexpr Optional(const internal::InvalidOpt &invalidOpt) noexcept;

        /**
        \param other
        The instance to copy from.

        \brief
        Copies the contents of \ilc{other} into the instance that is currently under construction.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be copied in any way; such copy will only occur if \ilc{other} is
        valid.
        */
        constexpr Optional(const Optional &other) noexcept(IsNothrowCopyConstructible<Type>::value);

        /**
        \param other
        The instance to move from.

        \brief
        Moves the contents of \ilc{other} into the instance that is currently under construction.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be moved in any way; such move operation will only occur if \ilc{other} is
        valid.

        Also, even if the wrapped object of \ilc{other} will be moved, \ilc{other} will stay valid.
        */
        inline Optional(Optional &&other) noexcept(IsNothrowMoveConstructible<Type>::value);

        /**
        \return
        \ilc{true} if the wrapped object is valid, \ilc{false} if not.

        \brief
        Returns whether the wrapped object is valid or not.
        */
        [[nodiscard]] constexpr boolean isValid() const noexcept;

        /**
        \return
        The wrapped object.

        \brief
        Returns the wrapped object.

        \warning
        The return value of this method is undefined if the wrapped object is not valid (\ilc{isValid()}
        returns \ilc{false}).

        \warning
        It is possible to set the value of the wrapped object by using this function. However, this is only
        valid if the object is already valid. Setting the value of an invalid value results in undefined
        behavior. To set the value of an invalid object, \ilc{set()} or the assignment operator can be used.

        \note
        This function does not transfer ownership of the wrapped object. The instance of \ilc{Optional} will
        still be the owner of it.
        */
        [[nodiscard]] inline Type &get() noexcept;

        /**
        \return
        The wrapped object.

        \brief
        Returns the wrapped object.

        \warning
        The return value of this method is undefined if the wrapped object is not valid (\ilc{isValid()}
        returns \ilc{false}).

        \note
        This function does not transfer ownership of the wrapped object. The instance of \ilc{Optional} will
        still be the owner of it.
        */
        [[nodiscard]] constexpr const Type &get() const noexcept;

        /**
        \param obj
        The object to return if the wrapped object is not valid.

        \return
        The wrapped object or \ilc{obj}.

        \brief
        Returns the wrapped object if it is valid or \ilc{obj} if the wrapped object is not valid.

        \warning
        It is possible to set the value of the wrapped object by using this function. However, if the object
        is not valid, the value of \ilc{obj} wil be changed instead. To set the value of an invalid object,
        \ilc{set()} or the assignment operator can be used.

        \note
        In contrast to \ilc{get()}, this function never returns an invalid object and, as a consequence, will
        never result in undefined behavior.

        \note
        This function does not transfer ownership of the wrapped object. The instance of \ilc{Optional} will
        still be the owner of it.
        */
        [[nodiscard]] inline Type &getOr(Type &obj) noexcept;

        /**
        \param obj
        The object to return if the wrapped object is not valid.

        \return
        The wrapped object or \ilc{obj}.

        \brief
        Returns the wrapped object if it is valid or \ilc{obj} if the wrapped object is not valid.

        \note
        In contrast to \ilc{get()}, this function never returns an invalid object and, as a consequence, will
        never result in undefined behavior.

        \note
        This function does not transfer ownership of the wrapped object. The instance of \ilc{Optional} will
        still be the owner of it.
        */
        [[nodiscard]] constexpr const Type &getOr(const Type &obj) const noexcept;

        /**
        \return
        The wrapped object.

        \brief
        Returns the wrapped object.

        \warning
        The return value of this method is undefined if the wrapped object is not valid (\ilc{isValid()}
        returns \ilc{false}).

        \note
        This function transfers ownership of the wrapped object. In any other way, it is the same as
        \ilc{get()}.

        \note
        After a call to this function, the wrapped object will still be valid (\ilc{isValid()} still returns
        \ilc{true}), but the wrapped object may be in an unusable state because its data was moved out of it.
        */
        [[nodiscard]] inline Type move() noexcept;

        /**
        \param obj
        The object to return if the wrapped object is not valid.

        \return
        The wrapped object or \ilc{obj}.

        \brief
        Returns the wrapped object if it is valid or \ilc{obj} if the wrapped object is not valid.

        \note
        In contrast to \ilc{move()}, this function never returns an invalid object and, as a consequence, will
        never result in undefined behavior.

        \note
        This function transfers ownership of the wrapped object. In any other way, it is the same as
        \ilc{getOr()}.

        \note
        After a call to this function, the wrapped object will still be valid (\ilc{isValid()} still returns
        \ilc{true}), but the wrapped object may be in an unusable state because its data was moved out of it.
        [This note only applies if the wrapped object was already valid before this function was called.]
        */
        [[nodiscard]] inline Type moveOr(Type &&obj) noexcept;

        /**
        \return
        A pointer to the wrapped object or \ilc{nullptr}.

        \brief
        Returns a pointer to the wrapped object or \ilc{nullptr}.

        \details
        If the wrapped object is valid, this function returns a pointer to that object. If it is not valid,
        \ilc{nullptr} will be returned.

        \warning
        It is possible to set the value of the wrapped object by using this function. However, if the object
        is not valid, the assignment of the new value will obviously fail because the returned pointer is
        \ilc{nullptr}. To set the value of an invalid object, \ilc{set()} or the assignment operator can be
        used.
        */
        [[nodiscard]] inline Type *ptr() noexcept;

        /**
        \return
        A pointer to the wrapped object or \ilc{nullptr}.

        \brief
        Returns a pointer to the wrapped object or \ilc{nullptr}.

        \details
        If the wrapped object is valid, this function returns a pointer to that object. If it is not valid,
        \ilc{nullptr} will be returned.
        */
        [[nodiscard]] inline const Type *ptr() const noexcept;

        /**
        \tparam ARGS
        The types of the parameters that will be used to construct the wrapped object. As a consequence, an
        instance of \ilc{Type} needs to be constructible from instances of these types.

        \param args
        The arguments that will be used to construct the wrapped object.

        \brief
        Sets the value of the wrapped object.

        \details
        This function can be used to set the value of an, to that point, invalid object. Doing so will
        validate the object and \ilc{isValid()} will return \ilc{true} afterwards.

        \note
        It should be avoided to continuously set the value of (already valid) wrapped objects. This is because
        this function will always destruct the wrapped object and then re-constrcut a new object. This
        function does \b not use copy- or move-constructors.
        */
        template<typename... ARGS, typename = EnableIfType<IsConstructible<T, ARGS...>::value>>
        inline void set(ARGS &&... args) noexcept(IsNothrowConstructible<Type, ARGS...>::value);

        /**
        \param invalidOpt
        A dummy parameter. Will not be used.

        \brief
        Resets the instance that the function is called on.

        \details
        Calling this function does the same as calling \ilc{reset()} would do.
        */
        inline void set(const internal::InvalidOpt &invalidOpt) noexcept;

        /**
        \brief
        Invalidates the wrapped object.

        \details
        If the wrapped object is valid, this will trigger the destruction of that object. After this function
        was called, \ilc{isValid()} will always return false.
        */
        inline void reset() noexcept(IsNothrowDestructible<Type>::value);

        /**
        \copydoc nou::Optional::get()
        */
        inline Type &operator*() noexcept;

        /**
        \copydoc nou::Optional::get() const
        */
        constexpr const Type &operator*() const noexcept;

        /**
        \copydoc nou::Optional::ptr()
        */
        inline Type *operator->() noexcept;

        /**
        \copydoc nou::Optional::ptr() const
        */
        inline const Type *operator->() const noexcept;

        /**
        \copydoc nou::Optional::set()

        \return
        \ret_selfref_op
        */
        template<typename... ARGS>
        inline Optional operator=(ARGS &&... args) noexcept(IsNothrowConstructible<Type, ARGS...>::value);
    };

    /**
    \return \ilc{internal::InvalidOpt::INSTANCE}.

    \brief
    Returns a special object that will always construct an instance of nou::Optional in an invalid state.

    \details
    The function can be used to explicitly state that an invalid instance of \ilc{nou::Optional} should be
    constructed.

    \par_example
    \code{.cpp}
    nou::Optional<nou::int32> func(nou::boolean b)
    {
        return b ? 5 : nou::invalidOpt();

        //the same as:
        //return b ? 5 : nou::Optional<nou::int32>();
    }
    \endcode

    \warning
    Using \ilc{auto} with \ilc{nou::invalidOpt()} will not result in an instance of \ilc{nou::Optional}.
    */
    constexpr const internal::InvalidOpt &invalidOpt();

    //========== Must be defined here to allow usage in constexpr constructors
    template<typename T>
    constexpr boolean Optional<T>::isValid() const noexcept
    {
        return Base::m_isValid;
    }

    template<typename T>
    constexpr auto Optional<T>::get() const noexcept -> const Type &
    {
        return Base::m_data;
    }
    //==========

    template<typename T, boolean TRIV_DEST>
    template<typename OT>
    constexpr internal::OptionalStorage<T, TRIV_DEST>::OptionalStorage(
        const DummyValid &,
        const Optional<OT> &other) noexcept(IsNothrowConstructible<Type, const OT &>::value) :
        m_data(other.get()),
        m_isValid(true)
    {}

    template<typename T, boolean TRIV_DEST>
    template<typename OT>
    constexpr internal::OptionalStorage<T, TRIV_DEST>::OptionalStorage(const DummyInvalid &,
                                                                       const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value) :
        m_dummy(),
        m_isValid(false)
    {}

    template<typename T, boolean TRIV_DEST>
    constexpr internal::OptionalStorage<T, TRIV_DEST>::OptionalStorage() noexcept :
        m_dummy(),
        m_isValid(false)
    {}

    template<typename T, boolean TRIV_DEST>
    template<typename... ARGS, typename>
    constexpr internal::OptionalStorage<T, TRIV_DEST>::OptionalStorage(ARGS &&... args) noexcept(
                IsNothrowConstructible<Type, ARGS...>::value) :
        m_data(forward<ARGS>(args)...),
        m_isValid(true)
    {}

    template<typename T, boolean TRIV_DEST>
    template<typename OT>
    constexpr internal::OptionalStorage<T, TRIV_DEST>::OptionalStorage(const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value) :
        OptionalStorage(other.isValid() ? OptionalStorage(DummyValid{}, other)
                                        : OptionalStorage(DummyInvalid{}, other))
    {}

    template<typename T>
    template<typename OT>
    constexpr internal::OptionalStorage<T, false>::OptionalStorage(const DummyValid &,
                                                                   const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value) :
        m_data(other.get()),
        m_isValid(true)
    {}

    template<typename T>
    template<typename OT>
    constexpr internal::OptionalStorage<T, false>::OptionalStorage(const DummyInvalid &,
                                                                   const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value) :
        m_dummy(),
        m_isValid(false)
    {}

    template<typename T>
    inline internal::OptionalStorage<T, false>::OptionalStorage() noexcept : m_dummy(), m_isValid(false)
    {}

    template<typename T>
    template<typename... ARGS, typename>
    inline internal::OptionalStorage<T, false>::OptionalStorage(ARGS &&... args) noexcept(
                IsNothrowConstructible<Type, ARGS...>::value) :
        m_data(forward<ARGS>(args)...),
        m_isValid(true)
    {}

    template<typename T>
    template<typename OT>
    inline internal::OptionalStorage<T, false>::OptionalStorage(const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value) :
        OptionalStorage(other.isValid() ? OptionalStorage(DummyValid{}, other)
                                        : OptionalStorage(DummyInvalid{}, other))
    {}

    template<typename T>
    void internal::OptionalStorage<T, false>::destroy() noexcept(IsNothrowDestructible<T>::value)
    {
        if(m_isValid)
            m_data.~Type();
    }

    template<typename T>
    internal::OptionalStorage<T, false>::~OptionalStorage() noexcept(IsNothrowDestructible<T>::value)
    {
        destroy();
    }

    template<typename T>
    constexpr Optional<T>::Optional() noexcept : Base()
    {}

    template<typename T>
    template<typename... ARGS, typename>
    constexpr Optional<T>::Optional(ARGS &&... args) noexcept(
                IsNothrowConstructible<Type, ARGS...>::value) : Base(forward<ARGS>(args)...)
    {}

    template<typename T>
    template<typename OT, typename>
    constexpr Optional<T>::Optional(const Optional<OT> &other) noexcept(
                IsNothrowConstructible<Type, const OT &>::value) : Base(other)
    {}

    template<typename T>
    template<typename OT, typename>
    inline Optional<T>::Optional(Optional<OT> &&other) noexcept(
                IsNothrowConstructible<Type, OT &&>::value)
    {
        if(other.isValid())
            set(other.get());
    }

    template<typename T>
    constexpr Optional<T>::Optional(const internal::InvalidOpt &invalidOpt) noexcept : Base()
    {}

    template<typename T>
    constexpr Optional<T>::Optional(const Optional<T> &other) noexcept(
                IsNothrowCopyConstructible<Type>::value) : Base(other)
    {}

    template<typename T>
    inline Optional<T>::Optional(Optional<T> &&other) noexcept(
                IsNothrowMoveConstructible<Type>::value)
    {
        if(other.isValid())
            set(other.get());
    }

    template<typename T>
    inline auto Optional<T>::get() noexcept -> Type &
    {
        return Base::m_data;
    }

    template<typename T>
    inline auto Optional<T>::getOr(Type &obj) noexcept -> Type &
    {
        return isValid() ? get() : obj;
    }

    template<typename T>
    constexpr auto Optional<T>::getOr(const Type &obj) const noexcept -> const Type &
    {
        return isValid() ? get() : obj;
    }

    template<typename T>
    inline auto Optional<T>::move() noexcept -> Type
    {
        return ::nou::move(get());
    }

    template<typename T>
    inline auto Optional<T>::moveOr(Type &&obj) noexcept -> Type
    {
        return isValid() ? ::nou::move(get()) : ::nou::move(obj);
    }

    template<typename T>
    inline auto Optional<T>::ptr() noexcept -> Type *
    {
        return isValid() ? &get() : nullptr;
    }

    template<typename T>
    inline auto Optional<T>::ptr() const noexcept -> const Type *
    {
        return isValid() ? &get() : nullptr;
    }

    template<typename T>
    template<typename... ARGS, typename>
    inline void Optional<T>::set(ARGS &&... args) noexcept(
                IsNothrowConstructible<Type, ARGS...>::value)
    {
        Base::destroy();

        new(&get()) Type(forward<ARGS>(args)...);
        Base::m_isValid = true;
    }

    template<typename T>
    inline void Optional<T>::set(const internal::InvalidOpt &invalidOpt) noexcept
    {
        reset();
    }

    template<typename T>
    inline void Optional<T>::reset() noexcept(IsNothrowDestructible<Type>::value)
    {
        Base::destroy();
        Base::m_isValid = false;
    }

    template<typename T>
    inline auto Optional<T>::operator*() noexcept -> Type &
    {
        return get();
    }

    template<typename T>
    constexpr auto Optional<T>::operator*() const noexcept -> const Type &
    {
        return get();
    }

    template<typename T>
    inline auto Optional<T>::operator-> () noexcept -> Type *
    {
        return ptr();
    }

    template<typename T>
    inline auto Optional<T>::operator-> () const noexcept -> const Type *
    {
        return ptr;
    }

    template<typename T>
    template<typename... ARGS>
    inline auto Optional<T>::operator=(ARGS &&... args) noexcept(
                IsNothrowConstructible<Type, ARGS...>::value) -> Optional<T>
    {
        set(forward<ARGS>(args)...);

        return *this;
    }

    constexpr const internal::InvalidOpt &invalidOpt()
    {
        return internal::INVALID_OPT_INSTANCE;
    }
} // namespace nou

#endif
