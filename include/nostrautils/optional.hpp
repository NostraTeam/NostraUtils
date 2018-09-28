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
    <li></li>
</ul>

\par_example
.....

For a more detailed example, see \link optional.ex.cpp here\endlink.
*/

/**
\example optional.ex.cpp

An example that demonstrates the usage of the optional component.
*/

// TODO: remove all static_cast<T&&>() with nou::move()

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
    namespace internal
    {
        struct InvalidOpt final
        {
            // empty dummy type
        };

        constexpr InvalidOpt INVALID_OPT_INSTANCE = InvalidOpt();
    } // namespace internal

    template<typename T>
    class Optional final
    {
    private:
        /**
        \brief
        A byte array that contains the wrapped object.
        */
        alignas(T) byte m_dataStorage[sizeof(T)];

        /**
        \brief
        A reference to \ilc{m_dataInstance.m_data}. For convenience.
        */
        T &m_data;

        /**
        \brief
        The valid state. \ilc{true} if valid, \ilc{false} if not.
        */
        boolean m_valid;

    public:
        /**
        \brief
        Constructs a new instance in an invalid state.
        */
        constexpr Optional() noexcept;

        /**
        \tparam ARGS
        The types of the parameter that will be used to construct the wrapped object. As a consequence, an
        instance of \ilc{T} needs to be constructible from instances of these types.

        \param args
        The arguments that will be used to construct the wrapped object. The values will be forwarded to the
        constructor of \ilc{T}.

        \brief
        Constructs a new instance in a valid state with a wrapped object that was constructed from the passed
        parameters.
         */
        template<typename... ARGS, typename = EnableIfType<IsConstructible<T, ARGS...>::value>>
        explicit constexpr Optional(ARGS &&... args) noexcept;

        /**
        \tparam OT
        The type of the object that the passed instance wraps around. \ilc{T} must be constructible from this
        type.

        \param other
        The instance that holds the object that will be copied into the new instance.

        \brief
        Copies the object of \ilc{other} into the one that is being constructed.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be copied in any way; such copy will only occur if \ilc{other} is valid.
        */
        template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
        constexpr Optional(const Optional<OT> &other) noexcept;

        /**
        \tparam OT
        The type of the object that the passed instance wraps around. \ilc{T} must be constructible from this
        type.

        \param other
        The instance that holds the object that will be moved into the new instance.

        \brief
        Moved the object of \ilc{other} into the one that is being constructed.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be moved in any way; such move operation will only occur if \ilc{other} is
        valid.
        */
        template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
        Optional(Optional<OT> &&other) noexcept;

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
        constexpr Optional(const Optional &other) noexcept;

        /**

        \param other
        The instance to move from.

        \brief
        Moves the contents of \ilc{other} into the instance that is currently under construction.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be moved in any way; such move operation will only occur if \ilc{other} is
        valid.
        */
        Optional(Optional &&other) noexcept;

        /**
        \return
        \ilc{true} if the wrapped object is valid, \ilc{false} if not.

        \brief
        Returns whether the wrapped object is valid or not.
        */
        constexpr boolean isValid() const noexcept;

        /**
        \return
        The wrapped object.

        \brief
        Returns the wrapped object.

        \warning
        The return value of this method is undefined if the wrapped object is not valid (\ilc{isValid()}
        returns false).
        */
        T &get() noexcept;

        /**
        \return
        The wrapped object.

        \brief
        Returns the wrapped object.

        \warning
        The return value of this method is undefined if the wrapped object is not valid (\ilc{isValid()}
        returns false).
        */
        constexpr const T &get() const noexcept;
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

    \warning
    Using \ilc{auto} with \ilc{nou::invalidOpt()} will not result in an instance of \ilc{nou::Optional}.

    \endcode
    */
    constexpr const internal::InvalidOpt &invalidOpt();

    template<typename T>
    constexpr Optional<T>::Optional() noexcept : m_data(*reinterpret_cast<T *>(m_dataStorage)), m_valid(false)
    {}

    template<typename T>
    template<typename... ARGS, typename>
    constexpr Optional<T>::Optional(ARGS &&... args) noexcept :
        m_data(*reinterpret_cast<T *>(m_dataStorage)),
        m_valid(true)
    {
        new(m_dataStorage) T(nou::forward<ARGS>(args)...);
    }

    template<typename T>
    template<typename OT, typename>
    constexpr Optional<T>::Optional(const Optional<OT> &other) noexcept :
        m_data(*reinterpret_cast<T *>(m_dataStorage)),
        m_valid(other.isValid())
    {
        if(m_valid)
            new(m_dataStorage) T(other.get());
    }

    template<typename T>
    template<typename OT, typename>
    Optional<T>::Optional(Optional<OT> &&other) noexcept :
        m_data(*reinterpret_cast<T *>(m_dataStorage)),
        m_valid(other.isValid())
    {
        if(m_valid)
            new(m_dataStorage) T(nou::move(other.get()));
    }

    template<typename T>
    constexpr Optional<T>::Optional(const internal::InvalidOpt &invalidOpt) noexcept :
        m_data(*reinterpret_cast<T *>(m_dataStorage)),
        m_valid(false)
    {}

    template<typename T>
    constexpr Optional<T>::Optional(const Optional &other) noexcept :
        m_data(*reinterpret_cast<T *>(m_dataStorage)),
        m_valid(other.m_valid)
    {
        if(m_valid)
            new(m_dataStorage) T(other.m_data);
    }

    template<typename T>
    Optional<T>::Optional(Optional &&other) noexcept :
        m_data(*reinterpret_cast<T *>(m_dataStorage)),
        m_valid(other.m_valid)
    {
        if(m_valid)
            new(m_dataStorage) T(nou::move(other.m_data));
    }

    template<typename T>
    constexpr boolean Optional<T>::isValid() const noexcept
    {
        return m_valid;
    }

    template<typename T>
    T &Optional<T>::get() noexcept
    {
        return m_data;
    }

    template<typename T>
    constexpr const T &Optional<T>::get() const noexcept
    {
        return m_data;
    }

    constexpr const internal::InvalidOpt &invalidOpt()
    {
        return internal::INVALID_OPT_INSTANCE;
    }
} // namespace nou

#endif
