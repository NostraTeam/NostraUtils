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

namespace nou
{
    namespace internal
    {
        struct InvalidOpt
        {
        public:
            constexpr static InvalidOpt INSTANCE;

        private:
            constexpr InvalidOpt() = default;
        };

        constexpr InvalidOpt InvalidOpt::INSTANCE;
    } // namespace internal

    template<typename T>
    class Optional final
    {
    private:
        /**
        \brief
        A union that contains the wrapped object.

        \details
        A union was chosen because it does not need to be constructed.

        It would also be possible to use a byte array (with the correct size and alignment).
        */
        union Data
        {
            T m_data;

            constexpr Data() = default;

            constexpr Data(const T &t);

            Data(T &&t);

            template<typename... ARGS, typename = EnableIfType<IsConstructible<T, ARGS...>::value>>
            constexpr Data(ARGS &&... args);
        };

        /**
        \brief
        A reference to \ilc{m_dataInstance.m_data}. For convenience.
        */
        T &m_data;

        /**
        \brief
        The wrapped object.
        */
        Data m_dataInstance;

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
        The type of the object that the passed instance wraps around.

        \param other
        The instance that holds the object that will be copied into the new instance.

        \brief
        Copies the object of \ilc{other} into the one that is being constructed.

        \details
        If \ilc{other} is not valid, the instance under construction will be initialized in an invalid state.
        The wrapped object will not be copied in any way; such copy will only occur if \ilc{other} is valid.
        */
        template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
        constexpr Optional(const Optional<OT> &other);

        /**
        \tparam OT
        The type of the object that the passed instance wraps around.

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
        Optional(Optional<OT> &&other);

        /**
        \param invalidOpt
        A dummy parameter. Will not be used.

        \brief
        Constructs a new instance in an invalid state.

        \details
        This constructor should never be used explicitly, its sole purpose is to be used with
        \ilc{nou::invalidOpt()}.
        */
        constexpr Optional(const internal::InvalidOpt &invalidOpt);

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
        constexpr Optional(const Optional &other);

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
        Optional(Optional &&other);
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
    constexpr Optional<T>::Data::Data(const T &t) : m_data(t)
    {}

    template<typename T>
    Optional<T>::Data::Data(T &&t) : m_data(static_cast<T &&>(t))
    {}

    template<typename T>
    template<typename... ARGS, typename>
    constexpr Optional<T>::Data::Data(ARGS &&... args) // TODO forward args
    {}

    template<typename T>
    constexpr Optional<T>::Optional() noexcept : m_data(m_dataInstance.m_data), m_valid(false)
    {}

    template<typename T>
    template<typename... ARGS, typename = EnableIfType<IsConstructible<T, ARGS...>::value>>
    explicit constexpr Optional<T>::Optional(ARGS &&... args) noexcept :
        m_data(m_dataInstance.m_data),
        m_dataInstance(), // TODO: forward args
        m_valid(true)
    {}

    template<typename T>
    template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
    constexpr Optional<T>::Optional(const Optional<OT> &other) :
        m_data(m_dataInstance.m_data),
        m_valid(other.m_valid),
        m_dataInstance(other.m_valid ? Data(other.m_data) : Data())
    {}

    template<typename T>
    template<typename OT, typename = EnableIfType<IsConstructible<T, OT>::value>>
    Optional<T>::Optional(Optional<OT> &&other) :
        m_data(m_dataInstance.m_data),
        m_valid(other.m_valid),
        m_dataInstance(other.m_valid ? Data(static_cast<OT &&>(other.m_data)) : Data())
    {}

    template<typename T>
    constexpr Optional<T>::Optional(const internal::InvalidOpt &invalidOpt) :
        m_data(m_dataInstance.m_data),
        m_valid(false)
    {}

    template<typename T>
    constexpr Optional<T>::Optional(const Optional &other) :
        m_data(m_dataInstance.m_data),
        m_valid(other.m_valid),
        m_dataInstance(other.m_valid ? Data(other.m_data) : Data())
    {}

    template<typename T>
    Optional<T>::Optional(Optional &&other) :
        m_data(m_dataInstance.m_data),
        m_valid(other.m_valid),
        m_dataInstance(other.m_valid ? Data(static_cast<T &&>(other.m_data)) : Data())
    {}

    constexpr const internal::InvalidOpt &invalidOpt()
    {
        return internal::InvalidOpt::INSTANCE;
    }
} // namespace nou

#endif
