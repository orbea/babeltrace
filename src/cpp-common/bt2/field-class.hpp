/*
 * Copyright (c) 2020 Philippe Proulx <pproulx@efficios.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef BABELTRACE_CPP_COMMON_BT2_FIELD_CLASS_HPP
#define BABELTRACE_CPP_COMMON_BT2_FIELD_CLASS_HPP

#include <type_traits>
#include <cstdint>
#include <babeltrace2/babeltrace.h>

#include "common/assert.h"
#include "internal/borrowed-obj.hpp"
#include "internal/shared-obj.hpp"
#include "cpp-common/optional.hpp"
#include "cpp-common/string_view.hpp"
#include "lib-error.hpp"
#include "integer-range-set.hpp"
#include "field-path.hpp"

namespace bt2 {

namespace internal {

struct FieldClassRefFuncs final
{
    static void get(const bt_field_class * const libObjPtr)
    {
        bt_field_class_get_ref(libObjPtr);
    }

    static void put(const bt_field_class * const libObjPtr)
    {
        bt_field_class_put_ref(libObjPtr);
    }
};

template <typename ObjT, typename LibObjT>
using SharedFieldClass = internal::SharedObj<ObjT, LibObjT, internal::FieldClassRefFuncs>;

template <typename LibObjT>
struct CommonFieldClassSpec;

// Functions specific to mutable field classes
template <>
struct CommonFieldClassSpec<bt_field_class> final
{
    static bt_value *userAttributes(bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_borrow_user_attributes(libObjPtr);
    }
};

// Functions specific to constant field classes
template <>
struct CommonFieldClassSpec<const bt_field_class> final
{
    static const bt_value *userAttributes(const bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_borrow_user_attributes_const(libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonBitArrayFieldClass;

template <typename LibObjT>
class CommonIntegerFieldClass;

template <typename LibObjT>
class ConstEnumerationFieldClassMapping;

template <typename LibObjT, typename MappingT>
class CommonEnumerationFieldClass;

template <typename LibObjT>
class CommonStructureFieldClass;

template <typename LibObjT>
class CommonArrayFieldClass;

template <typename LibObjT>
class CommonStaticArrayFieldClass;

template <typename LibObjT>
class CommonDynamicArrayWithLengthFieldClass;

template <typename LibObjT>
class CommonOptionFieldClass;

template <typename LibObjT>
class CommonOptionWithSelectorFieldClass;

template <typename LibObjT>
class CommonOptionWithBoolSelectorFieldClass;

template <typename LibObjT>
class CommonVariantFieldClass;

template <typename LibObjT, typename RangeSetT>
class CommonOptionWithIntegerSelectorFieldClass;

template <typename LibObjT>
class CommonVariantWithoutSelectorFieldClass;

template <typename LibObjT>
class ConstVariantWithIntegerSelectorFieldClassOption;

template <typename LibObjT, typename RangeSetT>
class CommonVariantWithIntegerSelectorFieldClass;

template <typename LibObjT>
class CommonEventClass;

template <typename LibObjT>
class CommonStreamClass;

template <typename LibObjT>
class CommonTraceClass;

enum class FieldClassType
{
    BOOL = BT_FIELD_CLASS_TYPE_BOOL,
    BIT_ARRAY = BT_FIELD_CLASS_TYPE_BIT_ARRAY,
    UNSIGNED_INTEGER = BT_FIELD_CLASS_TYPE_UNSIGNED_INTEGER,
    SIGNED_INTEGER = BT_FIELD_CLASS_TYPE_SIGNED_INTEGER,
    UNSIGNED_ENUMERATION = BT_FIELD_CLASS_TYPE_UNSIGNED_ENUMERATION,
    SIGNED_ENUMERATION = BT_FIELD_CLASS_TYPE_SIGNED_ENUMERATION,
    SINGLE_PRECISION_REAL = BT_FIELD_CLASS_TYPE_SINGLE_PRECISION_REAL,
    DOUBLE_PRECISION_REAL = BT_FIELD_CLASS_TYPE_DOUBLE_PRECISION_REAL,
    STRING = BT_FIELD_CLASS_TYPE_STRING,
    STRUCTURE = BT_FIELD_CLASS_TYPE_STRUCTURE,
    STATIC_ARRAY = BT_FIELD_CLASS_TYPE_STATIC_ARRAY,
    DYNAMIC_ARRAY_WITHOUT_LENGTH = BT_FIELD_CLASS_TYPE_DYNAMIC_ARRAY_WITHOUT_LENGTH_FIELD,
    DYNAMIC_ARRAY_WITH_LENGTH = BT_FIELD_CLASS_TYPE_DYNAMIC_ARRAY_WITH_LENGTH_FIELD,
    OPTION_WITHOUT_SELECTOR = BT_FIELD_CLASS_TYPE_OPTION_WITHOUT_SELECTOR_FIELD,
    OPTION_WITH_BOOL_SELECTOR = BT_FIELD_CLASS_TYPE_OPTION_WITH_BOOL_SELECTOR_FIELD,
    OPTION_WITH_UNSIGNED_INTEGER_SELECTOR =
        BT_FIELD_CLASS_TYPE_OPTION_WITH_UNSIGNED_INTEGER_SELECTOR_FIELD,
    OPTION_WITH_SIGNED_INTEGER_SELECTOR =
        BT_FIELD_CLASS_TYPE_OPTION_WITH_SIGNED_INTEGER_SELECTOR_FIELD,
    VARIANT_WITHOUT_SELECTOR = BT_FIELD_CLASS_TYPE_VARIANT_WITHOUT_SELECTOR_FIELD,
    VARIANT_WITH_UNSIGNED_INTEGER_SELECTOR =
        BT_FIELD_CLASS_TYPE_VARIANT_WITH_UNSIGNED_INTEGER_SELECTOR_FIELD,
    VARIANT_WITH_SIGNED_INTEGER_SELECTOR =
        BT_FIELD_CLASS_TYPE_VARIANT_WITH_SIGNED_INTEGER_SELECTOR_FIELD,
};

template <typename LibObjT>
class CommonFieldClass : public internal::BorrowedObj<LibObjT>
{
    // Allow appendMember() to call `fc._libObjPtr()`
    friend class CommonStructureFieldClass<bt_field_class>;

    // Allow appendOption() to call `fc._libObjPtr()`
    friend class CommonVariantWithoutSelectorFieldClass<bt_field_class>;

    friend class CommonVariantWithIntegerSelectorFieldClass<
        bt_field_class,
        ConstVariantWithIntegerSelectorFieldClassOption<
            const bt_field_class_variant_with_selector_field_integer_unsigned_option>>;

    friend class CommonVariantWithIntegerSelectorFieldClass<
        bt_field_class,
        ConstVariantWithIntegerSelectorFieldClassOption<
            const bt_field_class_variant_with_selector_field_integer_signed_option>>;

    // Allow *FieldClass() to call `fc._libObjPtr()`
    friend class CommonEventClass<bt_event_class>;
    friend class CommonStreamClass<bt_stream_class>;

    // Allow create*FieldClass() to call `fc._libObjPtr()`
    friend class CommonTraceClass<bt_trace_class>;

private:
    using typename internal::BorrowedObj<LibObjT>::_ThisBorrowedObj;

protected:
    using typename internal::BorrowedObj<LibObjT>::_LibObjPtr;
    using _ThisCommonFieldClass = CommonFieldClass<LibObjT>;

public:
    using Shared = internal::SharedFieldClass<CommonFieldClass<LibObjT>, LibObjT>;

    using UserAttributes =
        typename std::conditional<std::is_const<LibObjT>::value, ConstMapValue, MapValue>::type;

    explicit CommonFieldClass(const _LibObjPtr libObjPtr) noexcept : _ThisBorrowedObj {libObjPtr}
    {
    }

    template <typename OtherLibObjT>
    CommonFieldClass(const CommonFieldClass<OtherLibObjT>& fc) noexcept : _ThisBorrowedObj {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonFieldClass& operator=(const CommonFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisBorrowedObj::operator=(fc);
        return *this;
    }

    FieldClassType type() const noexcept
    {
        return static_cast<FieldClassType>(bt_field_class_get_type(this->_libObjPtr()));
    }

    bool isBool() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_BOOL);
    }

    bool isBitArray() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_BIT_ARRAY);
    }

    bool isInteger() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_INTEGER);
    }

    bool isUnsignedInteger() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_UNSIGNED_INTEGER);
    }

    bool isSignedInteger() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_SIGNED_INTEGER);
    }

    bool isEnumeration() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_ENUMERATION);
    }

    bool isUnsignedEnumeration() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_UNSIGNED_ENUMERATION);
    }

    bool isSignedEnumeration() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_SIGNED_ENUMERATION);
    }

    bool isSinglePrecisionReal() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_SINGLE_PRECISION_REAL);
    }

    bool isDoublePrecisionReal() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_DOUBLE_PRECISION_REAL);
    }

    bool isString() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_STRING);
    }

    bool isStructure() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_STRUCTURE);
    }

    bool isArray() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_ARRAY);
    }

    bool isStaticArray() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_STATIC_ARRAY);
    }

    bool isDynamicArray() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_DYNAMIC_ARRAY);
    }

    bool isDynamicArrayWithoutLength() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_DYNAMIC_ARRAY_WITHOUT_LENGTH_FIELD);
    }

    bool isDynamicArrayWithLength() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_DYNAMIC_ARRAY_WITH_LENGTH_FIELD);
    }

    bool isOption() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION);
    }

    bool isOptionWithoutSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION_WITHOUT_SELECTOR_FIELD);
    }

    bool isOptionWithSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION_WITH_SELECTOR_FIELD);
    }

    bool isOptionWithBoolSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION_WITH_BOOL_SELECTOR_FIELD);
    }

    bool isOptionWithIntegerSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION_WITH_INTEGER_SELECTOR_FIELD);
    }

    bool isOptionWithUnsignedIntegerSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION_WITH_UNSIGNED_INTEGER_SELECTOR_FIELD);
    }

    bool isOptionWithSignedIntegerSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_OPTION_WITH_SIGNED_INTEGER_SELECTOR_FIELD);
    }

    bool isVariant() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_VARIANT);
    }

    bool isVariantWithoutSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_VARIANT_WITHOUT_SELECTOR_FIELD);
    }

    bool isVariantWithSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_VARIANT_WITH_SELECTOR_FIELD);
    }

    bool isVariantWithIntegerSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_VARIANT_WITH_INTEGER_SELECTOR_FIELD);
    }

    bool isVariantWithUnsignedIntegerSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_VARIANT_WITH_UNSIGNED_INTEGER_SELECTOR_FIELD);
    }

    bool isVariantWithSignedIntegerSelector() const noexcept
    {
        return this->_libTypeIs(BT_FIELD_CLASS_TYPE_VARIANT_WITH_SIGNED_INTEGER_SELECTOR_FIELD);
    }

    CommonBitArrayFieldClass<LibObjT> asBitArray() const noexcept;
    CommonIntegerFieldClass<LibObjT> asInteger() const noexcept;

    CommonEnumerationFieldClass<LibObjT, ConstEnumerationFieldClassMapping<
                                             const bt_field_class_enumeration_unsigned_mapping>>
    asUnsignedEnumeration() const noexcept;

    CommonEnumerationFieldClass<
        LibObjT, ConstEnumerationFieldClassMapping<const bt_field_class_enumeration_signed_mapping>>
    asSignedEnumeration() const noexcept;

    CommonStructureFieldClass<LibObjT> asStructure() const noexcept;
    CommonArrayFieldClass<LibObjT> asArray() const noexcept;
    CommonStaticArrayFieldClass<LibObjT> asStaticArray() const noexcept;
    CommonDynamicArrayWithLengthFieldClass<LibObjT> asDynamicArrayWithLength() const noexcept;
    CommonOptionFieldClass<LibObjT> asOption() const noexcept;
    CommonOptionWithSelectorFieldClass<LibObjT> asOptionWithSelector() const noexcept;
    CommonOptionWithBoolSelectorFieldClass<LibObjT> asOptionWithBoolSelector() const noexcept;

    CommonOptionWithIntegerSelectorFieldClass<LibObjT, ConstUnsignedIntegerRangeSet>
    asOptionWithUnsignedIntegerSelector() const noexcept;

    CommonOptionWithIntegerSelectorFieldClass<LibObjT, ConstSignedIntegerRangeSet>
    asOptionWithSignedIntegerSelector() const noexcept;

    CommonVariantFieldClass<LibObjT> asVariant() const noexcept;
    CommonVariantWithoutSelectorFieldClass<LibObjT> asVariantWithoutSelector() const noexcept;

    CommonVariantWithIntegerSelectorFieldClass<
        LibObjT, ConstVariantWithIntegerSelectorFieldClassOption<
                     const bt_field_class_variant_with_selector_field_integer_unsigned_option>>
    asVariantWithUnsignedIntegerSelector() const noexcept;

    CommonVariantWithIntegerSelectorFieldClass<
        LibObjT, ConstVariantWithIntegerSelectorFieldClassOption<
                     const bt_field_class_variant_with_selector_field_integer_signed_option>>
    asVariantWithSignedIntegerSelector() const noexcept;

    template <typename LibValT>
    void userAttributes(const CommonMapValue<LibValT>& userAttrs)
    {
        static_assert(!std::is_const<LibObjT>::value, "`LibObjT` must NOT be `const`.");

        bt_field_class_set_user_attributes(this->_libObjPtr(), userAttrs._libObjPtr());
    }

    ConstMapValue userAttributes() const noexcept
    {
        return ConstMapValue {internal::CommonFieldClassSpec<const bt_field_class>::userAttributes(
            this->_libObjPtr())};
    }

    UserAttributes userAttributes() noexcept
    {
        return UserAttributes {
            internal::CommonFieldClassSpec<LibObjT>::userAttributes(this->_libObjPtr())};
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }

protected:
    bool _libTypeIs(const bt_field_class_type type) const noexcept
    {
        return bt_field_class_type_is(bt_field_class_get_type(this->_libObjPtr()), type);
    }
};

using FieldClass = CommonFieldClass<bt_field_class>;
using ConstFieldClass = CommonFieldClass<const bt_field_class>;

template <typename LibObjT>
class CommonBitArrayFieldClass final : public CommonFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using typename CommonFieldClass<LibObjT>::_ThisCommonFieldClass;

public:
    using Shared = internal::SharedFieldClass<CommonBitArrayFieldClass<LibObjT>, LibObjT>;

    explicit CommonBitArrayFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isBitArray());
    }

    template <typename OtherLibObjT>
    CommonBitArrayFieldClass(const CommonBitArrayFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonBitArrayFieldClass<LibObjT>&
    operator=(const CommonBitArrayFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonFieldClass::operator=(fc);
        return *this;
    }

    std::uint64_t length() const noexcept
    {
        return bt_field_class_bit_array_get_length(this->_libObjPtr());
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using BitArrayFieldClass = CommonBitArrayFieldClass<bt_field_class>;
using ConstBitArrayFieldClass = CommonBitArrayFieldClass<const bt_field_class>;

enum class DisplayBase
{
    BINARY = BT_FIELD_CLASS_INTEGER_PREFERRED_DISPLAY_BASE_BINARY,
    OCTAL = BT_FIELD_CLASS_INTEGER_PREFERRED_DISPLAY_BASE_OCTAL,
    DECIMAL = BT_FIELD_CLASS_INTEGER_PREFERRED_DISPLAY_BASE_DECIMAL,
    HEXADECIMAL = BT_FIELD_CLASS_INTEGER_PREFERRED_DISPLAY_BASE_HEXADECIMAL,
};

template <typename LibObjT>
class CommonIntegerFieldClass : public CommonFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_ThisCommonFieldClass;

protected:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using _ThisCommonIntegerFieldClass = CommonIntegerFieldClass<LibObjT>;

public:
    using Shared = internal::SharedFieldClass<CommonIntegerFieldClass<LibObjT>, LibObjT>;

    explicit CommonIntegerFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isInteger());
    }

    template <typename OtherLibObjT>
    CommonIntegerFieldClass(const CommonIntegerFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonIntegerFieldClass&
    operator=(const CommonIntegerFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonFieldClass::operator=(fc);
        return *this;
    }

    void fieldValueRange(const std::uint64_t n) noexcept
    {
        static_assert(!std::is_const<LibObjT>::value, "`LibObjT` must NOT be `const`.");

        bt_field_class_integer_get_field_value_range(this->_libObjPtr(), n);
    }

    std::uint64_t fieldValueRange() const noexcept
    {
        return bt_field_class_integer_get_field_value_range(this->_libObjPtr());
    }

    void preferredDisplayBase(const DisplayBase base) noexcept
    {
        static_assert(!std::is_const<LibObjT>::value, "`LibObjT` must NOT be `const`.");

        bt_field_class_integer_set_preferred_display_base(
            this->_libObjPtr(), static_cast<bt_field_class_integer_preferred_display_base>(base));
    }

    DisplayBase preferredDisplayBase() const noexcept
    {
        return static_cast<DisplayBase>(
            bt_field_class_integer_get_preferred_display_base(this->_libObjPtr()));
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using IntegerFieldClass = CommonIntegerFieldClass<bt_field_class>;
using ConstIntegerFieldClass = CommonIntegerFieldClass<const bt_field_class>;

namespace internal {

template <typename LibObjT>
struct ConstEnumerationFieldClassMappingSpec;

// Functions specific to unsigned enumeration field class mappings
template <>
struct ConstEnumerationFieldClassMappingSpec<const bt_field_class_enumeration_unsigned_mapping>
    final
{
    static const bt_integer_range_set_unsigned *
    ranges(const bt_field_class_enumeration_unsigned_mapping * const libObjPtr) noexcept
    {
        return bt_field_class_enumeration_unsigned_mapping_borrow_ranges_const(libObjPtr);
    }

    static const char *
    label(const bt_field_class_enumeration_unsigned_mapping * const libObjPtr) noexcept
    {
        return bt_field_class_enumeration_mapping_get_label(
            bt_field_class_enumeration_unsigned_mapping_as_mapping_const(libObjPtr));
    }
};

// Functions specific to signed enumeration field class mappings
template <>
struct ConstEnumerationFieldClassMappingSpec<const bt_field_class_enumeration_signed_mapping> final
{
    static const bt_integer_range_set_signed *
    ranges(const bt_field_class_enumeration_signed_mapping * const libObjPtr) noexcept
    {
        return bt_field_class_enumeration_signed_mapping_borrow_ranges_const(libObjPtr);
    }

    static const char *
    label(const bt_field_class_enumeration_signed_mapping * const libObjPtr) noexcept
    {
        return bt_field_class_enumeration_mapping_get_label(
            bt_field_class_enumeration_signed_mapping_as_mapping_const(libObjPtr));
    }
};

} // namespace internal

template <typename LibObjT>
class ConstEnumerationFieldClassMapping final : public internal::BorrowedObj<LibObjT>
{
private:
    using typename internal::BorrowedObj<LibObjT>::_ThisBorrowedObj;
    using typename internal::BorrowedObj<LibObjT>::_LibObjPtr;
    using _ThisConstEnumerationFieldClassMapping = ConstEnumerationFieldClassMapping<LibObjT>;

public:
    using RangeSet = typename std::conditional<
        std::is_same<LibObjT, const bt_field_class_enumeration_unsigned_mapping>::value,
        ConstUnsignedIntegerRangeSet, ConstSignedIntegerRangeSet>::type;

    explicit ConstEnumerationFieldClassMapping(const _LibObjPtr libObjPtr) noexcept :
        _ThisBorrowedObj {libObjPtr}
    {
    }

    ConstEnumerationFieldClassMapping(
        const _ThisConstEnumerationFieldClassMapping& mapping) noexcept :
        _ThisBorrowedObj {mapping}
    {
    }

    _ThisConstEnumerationFieldClassMapping&
    operator=(const _ThisConstEnumerationFieldClassMapping& mapping) noexcept
    {
        _ThisBorrowedObj::operator=(mapping);
        return *this;
    }

    RangeSet ranges() const noexcept
    {
        return RangeSet {
            internal::ConstEnumerationFieldClassMappingSpec<LibObjT>::ranges(this->_libObjPtr())};
    }

    bpstd::string_view label() const noexcept
    {
        return internal::ConstEnumerationFieldClassMappingSpec<LibObjT>::label(this->_libObjPtr());
    }
};

using ConstUnsignedEnumerationFieldClassMapping =
    ConstEnumerationFieldClassMapping<const bt_field_class_enumeration_unsigned_mapping>;

using ConstSignedEnumerationFieldClassMapping =
    ConstEnumerationFieldClassMapping<const bt_field_class_enumeration_signed_mapping>;

namespace internal {

template <typename MappingT>
struct CommonEnumerationFieldClassSpec;

// Functions specific to unsigned enumeration field classes
template <>
struct CommonEnumerationFieldClassSpec<ConstUnsignedEnumerationFieldClassMapping> final
{
    static const bt_field_class_enumeration_unsigned_mapping *
    mappingByIndex(const bt_field_class * const libObjPtr, const std::uint64_t index) noexcept
    {
        return bt_field_class_enumeration_unsigned_borrow_mapping_by_index_const(libObjPtr, index);
    }

    static const bt_field_class_enumeration_unsigned_mapping *
    mappingByLabel(const bt_field_class * const libObjPtr, const char * const label) noexcept
    {
        return bt_field_class_enumeration_unsigned_borrow_mapping_by_label_const(libObjPtr, label);
    }
};

// Functions specific to signed enumeration field classes
template <>
struct CommonEnumerationFieldClassSpec<ConstSignedEnumerationFieldClassMapping> final
{
    static const bt_field_class_enumeration_signed_mapping *
    mappingByIndex(const bt_field_class * const libObjPtr, const std::uint64_t index) noexcept
    {
        return bt_field_class_enumeration_signed_borrow_mapping_by_index_const(libObjPtr, index);
    }

    static const bt_field_class_enumeration_signed_mapping *
    mappingByLabel(const bt_field_class * const libObjPtr, const char * const label) noexcept
    {
        return bt_field_class_enumeration_signed_borrow_mapping_by_label_const(libObjPtr, label);
    }
};

} // namespace internal

template <typename LibObjT, typename MappingT>
class CommonEnumerationFieldClass final : public CommonIntegerFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using typename CommonIntegerFieldClass<LibObjT>::_ThisCommonIntegerFieldClass;
    using _ThisCommonEnumerationFieldClass = CommonEnumerationFieldClass<LibObjT, MappingT>;

public:
    using Shared = internal::SharedFieldClass<_ThisCommonEnumerationFieldClass, LibObjT>;
    using Mapping = MappingT;

    explicit CommonEnumerationFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonIntegerFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isEnumeration());
    }

    template <typename OtherLibObjT>
    CommonEnumerationFieldClass(
        const CommonEnumerationFieldClass<OtherLibObjT, MappingT>& fc) noexcept :
        _ThisCommonIntegerFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonEnumerationFieldClass&
    operator=(const CommonEnumerationFieldClass<OtherLibObjT, MappingT>& fc) noexcept
    {
        _ThisCommonIntegerFieldClass::operator=(fc);
        return *this;
    }

    std::uint64_t size() const noexcept
    {
        return bt_field_class_enumeration_get_mapping_count(this->_libObjPtr());
    }

    Mapping operator[](const std::uint64_t index) const noexcept
    {
        return Mapping {internal::CommonEnumerationFieldClassSpec<MappingT>::mappingByIndex(
            this->_libObjPtr(), index)};
    }

    nonstd::optional<Mapping> operator[](const char * const label) const noexcept
    {
        const auto libObjPtr = internal::CommonEnumerationFieldClassSpec<MappingT>::mappingByLabel(
            this->_libObjPtr(), label);

        if (libObjPtr) {
            return Mapping {libObjPtr};
        }

        return nonstd::nullopt;
    }

    nonstd::optional<Mapping> operator[](const std::string& label) const noexcept
    {
        return (*this)[label.data()];
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using UnsignedEnumerationFieldClass =
    CommonEnumerationFieldClass<bt_field_class, ConstUnsignedEnumerationFieldClassMapping>;

using ConstUnsignedEnumerationFieldClass =
    CommonEnumerationFieldClass<const bt_field_class, ConstUnsignedEnumerationFieldClassMapping>;

using SignedEnumerationFieldClass =
    CommonEnumerationFieldClass<bt_field_class, ConstSignedEnumerationFieldClassMapping>;

using ConstSignedEnumerationFieldClass =
    CommonEnumerationFieldClass<const bt_field_class, ConstSignedEnumerationFieldClassMapping>;

namespace internal {

template <typename LibObjT>
struct CommonStructureFieldClassMemberSpec;

// Functions specific to mutable structure field class members
template <>
struct CommonStructureFieldClassMemberSpec<bt_field_class_structure_member> final
{
    static bt_field_class *fieldClass(bt_field_class_structure_member * const libObjPtr) noexcept
    {
        return bt_field_class_structure_member_borrow_field_class(libObjPtr);
    }
};

// Functions specific to constant structure field class members
template <>
struct CommonStructureFieldClassMemberSpec<const bt_field_class_structure_member> final
{
    static const bt_field_class *
    fieldClass(const bt_field_class_structure_member * const libObjPtr) noexcept
    {
        return bt_field_class_structure_member_borrow_field_class_const(libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonStructureFieldClassMember final : public internal::BorrowedObj<LibObjT>
{
private:
    using typename internal::BorrowedObj<LibObjT>::_LibObjPtr;
    using typename internal::BorrowedObj<LibObjT>::_ThisBorrowedObj;

    using _FieldClass =
        typename std::conditional<std::is_const<LibObjT>::value, ConstFieldClass, FieldClass>::type;

public:
    explicit CommonStructureFieldClassMember(const _LibObjPtr libObjPtr) noexcept :
        _ThisBorrowedObj {libObjPtr}
    {
    }

    template <typename OtherLibObjT>
    CommonStructureFieldClassMember(
        const CommonStructureFieldClassMember<OtherLibObjT>& fc) noexcept :
        _ThisBorrowedObj {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonStructureFieldClassMember<LibObjT>&
    operator=(const CommonStructureFieldClassMember<OtherLibObjT>& fc) noexcept
    {
        _ThisBorrowedObj::operator=(fc);
        return *this;
    }

    bpstd::string_view name() const noexcept
    {
        return bt_field_class_structure_member_get_name(this->_libObjPtr());
    }

    ConstFieldClass fieldClass() const noexcept
    {
        return ConstFieldClass {internal::CommonStructureFieldClassMemberSpec<
            const bt_field_class_structure_member>::fieldClass(this->_libObjPtr())};
    }

    _FieldClass fieldClass() noexcept
    {
        return _FieldClass {
            internal::CommonStructureFieldClassMemberSpec<LibObjT>::fieldClass(this->_libObjPtr())};
    }
};

using StructureFieldClassMember = CommonStructureFieldClassMember<bt_field_class_structure_member>;

using ConstStructureFieldClassMember =
    CommonStructureFieldClassMember<const bt_field_class_structure_member>;

namespace internal {

template <typename LibObjT>
struct CommonStructureFieldClassSpec;

// Functions specific to mutable structure field classes
template <>
struct CommonStructureFieldClassSpec<bt_field_class> final
{
    static bt_field_class_structure_member *memberByIndex(bt_field_class * const libObjPtr,
                                                          const std::uint64_t index) noexcept
    {
        return bt_field_class_structure_borrow_member_by_index(libObjPtr, index);
    }

    static bt_field_class_structure_member *memberByName(bt_field_class * const libObjPtr,
                                                         const char * const name) noexcept
    {
        return bt_field_class_structure_borrow_member_by_name(libObjPtr, name);
    }
};

// Functions specific to constant structure field classes
template <>
struct CommonStructureFieldClassSpec<const bt_field_class> final
{
    static const bt_field_class_structure_member *
    memberByIndex(const bt_field_class * const libObjPtr, const std::uint64_t index) noexcept
    {
        return bt_field_class_structure_borrow_member_by_index_const(libObjPtr, index);
    }

    static const bt_field_class_structure_member *
    memberByName(const bt_field_class * const libObjPtr, const char * const name) noexcept
    {
        return bt_field_class_structure_borrow_member_by_name_const(libObjPtr, name);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonStructureFieldClass final : public CommonFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using typename CommonFieldClass<LibObjT>::_ThisCommonFieldClass;

public:
    using Shared = internal::SharedFieldClass<CommonStructureFieldClass<LibObjT>, LibObjT>;

    using Member =
        typename std::conditional<std::is_const<LibObjT>::value, ConstStructureFieldClassMember,
                                  StructureFieldClassMember>::type;

    explicit CommonStructureFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isStructure());
    }

    template <typename OtherLibObjT>
    CommonStructureFieldClass(const CommonStructureFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonStructureFieldClass<LibObjT>&
    operator=(const CommonStructureFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonFieldClass::operator=(fc);
        return *this;
    }

    void appendMember(const char * const name, const FieldClass& fc)
    {
        static_assert(!std::is_const<LibObjT>::value, "`LibObjT` must NOT be `const`.");

        const auto status =
            bt_field_class_structure_append_member(this->_libObjPtr(), name, fc._libObjPtr());

        if (status == BT_FIELD_CLASS_STRUCTURE_APPEND_MEMBER_STATUS_MEMORY_ERROR) {
            throw LibMemoryError {};
        }
    }

    void appendMember(const std::string& name, const FieldClass& fc)
    {
        this->appendMember(name.data(), fc);
    }

    std::uint64_t size() const noexcept
    {
        return bt_field_class_structure_get_member_count(this->_libObjPtr());
    }

    ConstStructureFieldClassMember operator[](const std::uint64_t index) const noexcept
    {
        return ConstStructureFieldClassMember {
            internal::CommonStructureFieldClassSpec<const bt_field_class>::memberByIndex(
                this->_libObjPtr(), index)};
    }

    Member operator[](const std::uint64_t index) noexcept
    {
        return Member {internal::CommonStructureFieldClassSpec<LibObjT>::memberByIndex(
            this->_libObjPtr(), index)};
    }

    nonstd::optional<ConstStructureFieldClassMember>
    operator[](const char * const name) const noexcept
    {
        const auto libObjPtr =
            internal::CommonStructureFieldClassSpec<const bt_field_class>::memberByName(
                this->_libObjPtr(), name);

        if (libObjPtr) {
            return ConstStructureFieldClassMember {libObjPtr};
        }

        return nonstd::nullopt;
    }

    nonstd::optional<ConstStructureFieldClassMember>
    operator[](const std::string& name) const noexcept
    {
        return (*this)[name.data()];
    }

    nonstd::optional<Member> operator[](const char * const name) noexcept
    {
        const auto libObjPtr = internal::CommonStructureFieldClassSpec<LibObjT>::memberByName(
            this->_libObjPtr(), name);

        if (libObjPtr) {
            return Member {libObjPtr};
        }

        return nonstd::nullopt;
    }

    nonstd::optional<Member> operator[](const std::string& name) noexcept
    {
        return (*this)[name.data()];
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using StructureFieldClass = CommonStructureFieldClass<bt_field_class>;
using ConstStructureFieldClass = CommonStructureFieldClass<const bt_field_class>;

namespace internal {

template <typename LibObjT>
struct CommonArrayFieldClassSpec;

// Functions specific to mutable array field classes
template <>
struct CommonArrayFieldClassSpec<bt_field_class> final
{
    static bt_field_class *elementFieldClass(bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_array_borrow_element_field_class(libObjPtr);
    }
};

// Functions specific to constant array field classes
template <>
struct CommonArrayFieldClassSpec<const bt_field_class> final
{
    static const bt_field_class *elementFieldClass(const bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_array_borrow_element_field_class_const(libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonArrayFieldClass : public CommonFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_ThisCommonFieldClass;

    using _FieldClass =
        typename std::conditional<std::is_const<LibObjT>::value, ConstFieldClass, FieldClass>::type;

protected:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using _ThisCommonArrayFieldClass = CommonArrayFieldClass<LibObjT>;

public:
    using Shared = internal::SharedFieldClass<CommonArrayFieldClass<LibObjT>, LibObjT>;

    explicit CommonArrayFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isArray());
    }

    template <typename OtherLibObjT>
    CommonArrayFieldClass(const CommonArrayFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonArrayFieldClass& operator=(const CommonArrayFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonFieldClass::operator=(fc);
        return *this;
    }

    ConstFieldClass elementFieldClass() const noexcept
    {
        return ConstFieldClass {
            internal::CommonArrayFieldClassSpec<const bt_field_class>::elementFieldClass(
                this->_libObjPtr())};
    }

    _FieldClass elementFieldClass() noexcept
    {
        return _FieldClass {
            internal::CommonArrayFieldClassSpec<LibObjT>::elementFieldClass(this->_libObjPtr())};
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using ArrayFieldClass = CommonArrayFieldClass<bt_field_class>;
using ConstArrayFieldClass = CommonArrayFieldClass<const bt_field_class>;

template <typename LibObjT>
class CommonStaticArrayFieldClass final : public CommonArrayFieldClass<LibObjT>
{
private:
    using typename CommonArrayFieldClass<LibObjT>::_ThisCommonArrayFieldClass;
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;

public:
    using Shared = internal::SharedFieldClass<CommonStaticArrayFieldClass<LibObjT>, LibObjT>;

    explicit CommonStaticArrayFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonArrayFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isStaticArray());
    }

    template <typename OtherLibObjT>
    CommonStaticArrayFieldClass(const CommonStaticArrayFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonArrayFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonStaticArrayFieldClass<LibObjT>&
    operator=(const CommonStaticArrayFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonArrayFieldClass::operator=(fc);
        return *this;
    }

    std::uint64_t length() const noexcept
    {
        return bt_field_class_array_static_get_length(this->_libObjPtr());
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using StaticArrayFieldClass = CommonStaticArrayFieldClass<bt_field_class>;
using ConstStaticArrayFieldClass = CommonStaticArrayFieldClass<const bt_field_class>;

template <typename LibObjT>
class CommonDynamicArrayWithLengthFieldClass final : public CommonArrayFieldClass<LibObjT>
{
private:
    using typename CommonArrayFieldClass<LibObjT>::_ThisCommonArrayFieldClass;
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;

public:
    using Shared =
        internal::SharedFieldClass<CommonDynamicArrayWithLengthFieldClass<LibObjT>, LibObjT>;

    explicit CommonDynamicArrayWithLengthFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonArrayFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isDynamicArrayWithLength());
    }

    template <typename OtherLibObjT>
    CommonDynamicArrayWithLengthFieldClass(
        const CommonDynamicArrayWithLengthFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonArrayFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonDynamicArrayWithLengthFieldClass<LibObjT>&
    operator=(const CommonDynamicArrayWithLengthFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonArrayFieldClass::operator=(fc);
        return *this;
    }

    ConstFieldPath lengthFieldPath() const noexcept
    {
        return ConstFieldPath {
            bt_field_class_array_dynamic_with_length_field_borrow_length_field_path_const(
                this->_libObjPtr())};
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using DynamicArrayWithLengthFieldClass = CommonDynamicArrayWithLengthFieldClass<bt_field_class>;

using ConstDynamicArrayWithLengthFieldClass =
    CommonDynamicArrayWithLengthFieldClass<const bt_field_class>;

namespace internal {

template <typename LibObjT>
struct CommonOptionFieldClassSpec;

// Functions specific to mutable option field classes
template <>
struct CommonOptionFieldClassSpec<bt_field_class> final
{
    static bt_field_class *fieldClass(bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_option_borrow_field_class(libObjPtr);
    }
};

// Functions specific to constant option field classes
template <>
struct CommonOptionFieldClassSpec<const bt_field_class> final
{
    static const bt_field_class *fieldClass(const bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_option_borrow_field_class_const(libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonOptionFieldClass : public CommonFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_ThisCommonFieldClass;

    using _FieldClass =
        typename std::conditional<std::is_const<LibObjT>::value, ConstFieldClass, FieldClass>::type;

protected:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using _ThisCommonOptionFieldClass = CommonOptionFieldClass<LibObjT>;

public:
    using Shared = internal::SharedFieldClass<CommonOptionFieldClass<LibObjT>, LibObjT>;

    explicit CommonOptionFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isOption());
    }

    template <typename OtherLibObjT>
    CommonOptionFieldClass(const CommonOptionFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonOptionFieldClass& operator=(const CommonOptionFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonFieldClass::operator=(fc);
        return *this;
    }

    ConstFieldClass fieldClass() const noexcept
    {
        return ConstFieldClass {
            internal::CommonOptionFieldClassSpec<const bt_field_class>::fieldClass(
                this->_libObjPtr())};
    }

    _FieldClass fieldClass() noexcept
    {
        return _FieldClass {
            internal::CommonOptionFieldClassSpec<LibObjT>::fieldClass(this->_libObjPtr())};
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using OptionFieldClass = CommonOptionFieldClass<bt_field_class>;
using ConstOptionFieldClass = CommonOptionFieldClass<const bt_field_class>;

template <typename LibObjT>
class CommonOptionWithSelectorFieldClass : public CommonOptionFieldClass<LibObjT>
{
private:
    using typename CommonOptionFieldClass<LibObjT>::_ThisCommonOptionFieldClass;

protected:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using _ThisCommonOptionWithSelectorFieldClass = CommonOptionWithSelectorFieldClass<LibObjT>;

public:
    using Shared = internal::SharedFieldClass<CommonOptionWithSelectorFieldClass<LibObjT>, LibObjT>;

    explicit CommonOptionWithSelectorFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonOptionFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isOptionWithSelector());
    }

    template <typename OtherLibObjT>
    CommonOptionWithSelectorFieldClass(
        const CommonOptionWithSelectorFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonOptionFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonOptionWithSelectorFieldClass&
    operator=(const CommonOptionWithSelectorFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonOptionFieldClass::operator=(fc);
        return *this;
    }

    ConstFieldPath selectorFieldPath() const noexcept
    {
        return ConstFieldPath {
            bt_field_class_option_with_selector_field_borrow_selector_field_path_const(
                this->_libObjPtr())};
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using OptionWithSelectorFieldClass = CommonOptionWithSelectorFieldClass<bt_field_class>;
using ConstOptionWithSelectorFieldClass = CommonOptionWithSelectorFieldClass<const bt_field_class>;

template <typename LibObjT>
class CommonOptionWithBoolSelectorFieldClass : public CommonOptionWithSelectorFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;

    using typename CommonOptionWithSelectorFieldClass<
        LibObjT>::_ThisCommonOptionWithSelectorFieldClass;

public:
    using Shared =
        internal::SharedFieldClass<CommonOptionWithBoolSelectorFieldClass<LibObjT>, LibObjT>;

    explicit CommonOptionWithBoolSelectorFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonOptionWithSelectorFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isOptionWithBoolSelector());
    }

    template <typename OtherLibObjT>
    CommonOptionWithBoolSelectorFieldClass(
        const CommonOptionWithBoolSelectorFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonOptionWithSelectorFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonOptionWithBoolSelectorFieldClass<LibObjT>&
    operator=(const CommonOptionWithBoolSelectorFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonOptionWithSelectorFieldClass::operator=(fc);
        return *this;
    }

    bool selectorIsReversed() const noexcept
    {
        return bt_field_class_option_with_selector_field_bool_selector_is_reversed(
            this->_libObjPtr());
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using OptionWithBoolSelectorFieldClass = CommonOptionWithBoolSelectorFieldClass<bt_field_class>;

using ConstOptionWithBoolSelectorFieldClass =
    CommonOptionWithBoolSelectorFieldClass<const bt_field_class>;

namespace internal {

template <typename RangeSetT>
struct CommonOptionWithIntegerSelectorFieldClassSpec;

// Functions specific to option field classes with unsigned integer ranges
template <>
struct CommonOptionWithIntegerSelectorFieldClassSpec<ConstUnsignedIntegerRangeSet> final
{
    static const bt_integer_range_set_unsigned *
    ranges(const bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_option_with_selector_field_integer_unsigned_borrow_selector_ranges_const(
            libObjPtr);
    }
};

// Functions specific to option field classes with signed ranges
template <>
struct CommonOptionWithIntegerSelectorFieldClassSpec<ConstSignedIntegerRangeSet> final
{
    static const bt_integer_range_set_signed *
    ranges(const bt_field_class * const libObjPtr) noexcept
    {
        return bt_field_class_option_with_selector_field_integer_signed_borrow_selector_ranges_const(
            libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT, typename RangeSetT>
class CommonOptionWithIntegerSelectorFieldClass : public CommonOptionWithSelectorFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;

    using typename CommonOptionWithSelectorFieldClass<
        LibObjT>::_ThisCommonOptionWithSelectorFieldClass;

    using _ThisCommonOptionWithIntegerSelectorFieldClass =
        CommonOptionWithIntegerSelectorFieldClass<LibObjT, RangeSetT>;

public:
    using Shared =
        internal::SharedFieldClass<_ThisCommonOptionWithIntegerSelectorFieldClass, LibObjT>;

    using RangeSet = RangeSetT;

    explicit CommonOptionWithIntegerSelectorFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonOptionWithSelectorFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isOptionWithIntegerSelector());
    }

    template <typename OtherLibObjT>
    CommonOptionWithIntegerSelectorFieldClass(
        const CommonOptionWithIntegerSelectorFieldClass<OtherLibObjT, RangeSetT>& fc) noexcept :
        _ThisCommonOptionWithSelectorFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonOptionWithIntegerSelectorFieldClass&
    operator=(const CommonOptionWithIntegerSelectorFieldClass<OtherLibObjT, RangeSetT>& fc) noexcept
    {
        _ThisCommonOptionWithSelectorFieldClass::operator=(fc);
        return *this;
    }

    RangeSet ranges() const noexcept
    {
        return RangeSet {internal::CommonOptionWithIntegerSelectorFieldClassSpec<RangeSetT>::ranges(
            this->_libObjPtr())};
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using OptionWithUnsignedIntegerSelectorFieldClass =
    CommonOptionWithIntegerSelectorFieldClass<bt_field_class, ConstUnsignedIntegerRangeSet>;

using ConstOptionWithUnsignedIntegerSelectorFieldClass =
    CommonOptionWithIntegerSelectorFieldClass<const bt_field_class, ConstUnsignedIntegerRangeSet>;

using OptionWithSignedIntegerSelectorFieldClass =
    CommonOptionWithIntegerSelectorFieldClass<bt_field_class, ConstSignedIntegerRangeSet>;

using ConstOptionWithSignedIntegerSelectorFieldClass =
    CommonOptionWithIntegerSelectorFieldClass<const bt_field_class, ConstSignedIntegerRangeSet>;

namespace internal {

template <typename LibObjT>
struct CommonVariantFieldClassOptionSpec;

// Functions specific to mutable variant field class options
template <>
struct CommonVariantFieldClassOptionSpec<bt_field_class_variant_option> final
{
    static bt_field_class *fieldClass(bt_field_class_variant_option * const libObjPtr) noexcept
    {
        return bt_field_class_variant_option_borrow_field_class(libObjPtr);
    }
};

// Functions specific to constant variant field class options
template <>
struct CommonVariantFieldClassOptionSpec<const bt_field_class_variant_option> final
{
    static const bt_field_class *
    fieldClass(const bt_field_class_variant_option * const libObjPtr) noexcept
    {
        return bt_field_class_variant_option_borrow_field_class_const(libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonVariantFieldClassOption : public internal::BorrowedObj<LibObjT>
{
private:
    using typename internal::BorrowedObj<LibObjT>::_ThisBorrowedObj;
    using typename internal::BorrowedObj<LibObjT>::_LibObjPtr;

    using _FieldClass =
        typename std::conditional<std::is_const<LibObjT>::value, ConstFieldClass, FieldClass>::type;

public:
    explicit CommonVariantFieldClassOption(const _LibObjPtr libObjPtr) noexcept :
        _ThisBorrowedObj {libObjPtr}
    {
    }

    template <typename OtherLibObjT>
    CommonVariantFieldClassOption(const CommonVariantFieldClassOption<OtherLibObjT>& fc) noexcept :
        _ThisBorrowedObj {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonVariantFieldClassOption<LibObjT>&
    operator=(const CommonVariantFieldClassOption<OtherLibObjT>& fc) noexcept
    {
        _ThisBorrowedObj::operator=(fc);
        return *this;
    }

    bpstd::string_view name() const noexcept
    {
        return bt_field_class_variant_option_get_name(this->_libObjPtr());
    }

    ConstFieldClass fieldClass() const noexcept
    {
        return ConstFieldClass {internal::CommonVariantFieldClassOptionSpec<
            const bt_field_class_variant_option>::fieldClass(this->_libObjPtr())};
    }

    _FieldClass fieldClass() noexcept
    {
        return _FieldClass {
            internal::CommonVariantFieldClassOptionSpec<LibObjT>::fieldClass(this->_libObjPtr())};
    }
};

using VariantFieldClassOption = CommonVariantFieldClassOption<bt_field_class_variant_option>;

using ConstVariantFieldClassOption =
    CommonVariantFieldClassOption<const bt_field_class_variant_option>;

namespace internal {

template <typename LibObjT>
struct ConstVariantWithIntegerSelectorFieldClassOptionSpec;

// Functions specific to variant field class options with unsigned integer selector
template <>
struct ConstVariantWithIntegerSelectorFieldClassOptionSpec<
    const bt_field_class_variant_with_selector_field_integer_unsigned_option>
    final
{
    static const bt_integer_range_set_unsigned *
    ranges(const bt_field_class_variant_with_selector_field_integer_unsigned_option
               * const libObjPtr) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_unsigned_option_borrow_ranges_const(
            libObjPtr);
    }

    static const bt_field_class_variant_option *
    asBaseOption(const bt_field_class_variant_with_selector_field_integer_unsigned_option
                     * const libObjPtr) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_unsigned_option_as_option_const(
            libObjPtr);
    }
};

// Functions specific to variant field class options with signed integer selector
template <>
struct ConstVariantWithIntegerSelectorFieldClassOptionSpec<
    const bt_field_class_variant_with_selector_field_integer_signed_option>
    final
{
    static const bt_integer_range_set_signed *
    ranges(const bt_field_class_variant_with_selector_field_integer_signed_option
               * const libObjPtr) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_signed_option_borrow_ranges_const(
            libObjPtr);
    }

    static const bt_field_class_variant_option *
    asBaseOption(const bt_field_class_variant_with_selector_field_integer_signed_option
                     * const libObjPtr) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_signed_option_as_option_const(
            libObjPtr);
    }
};

} // namespace internal

template <typename LibObjT>
class ConstVariantWithIntegerSelectorFieldClassOption : public internal::BorrowedObj<LibObjT>
{
private:
    using typename internal::BorrowedObj<LibObjT>::_ThisBorrowedObj;
    using typename internal::BorrowedObj<LibObjT>::_LibObjPtr;
    using _Spec = internal::ConstVariantWithIntegerSelectorFieldClassOptionSpec<LibObjT>;

public:
    using RangeSet = typename std::conditional<
        std::is_same<
            LibObjT,
            const bt_field_class_variant_with_selector_field_integer_unsigned_option>::value,
        ConstUnsignedIntegerRangeSet, ConstSignedIntegerRangeSet>::type;

    explicit ConstVariantWithIntegerSelectorFieldClassOption(const _LibObjPtr libObjPtr) noexcept :
        _ThisBorrowedObj {libObjPtr}
    {
    }

    template <typename OtherLibObjT>
    ConstVariantWithIntegerSelectorFieldClassOption(
        const ConstVariantWithIntegerSelectorFieldClassOption<OtherLibObjT>& fc) noexcept :
        _ThisBorrowedObj {fc}
    {
    }

    template <typename OtherLibObjT>
    ConstVariantWithIntegerSelectorFieldClassOption<LibObjT>&
    operator=(const ConstVariantWithIntegerSelectorFieldClassOption<OtherLibObjT>& fc) noexcept
    {
        _ThisBorrowedObj::operator=(fc);
        return *this;
    }

    ConstVariantFieldClassOption asBaseOption() const noexcept
    {
        return ConstVariantFieldClassOption {_Spec::asBaseOption(this->_libObjPtr())};
    }

    bpstd::string_view name() const noexcept
    {
        return this->asBaseOption().name();
    }

    ConstFieldClass fieldClass() const noexcept
    {
        return this->asBaseOption().fieldClass();
    }

    RangeSet ranges() const noexcept
    {
        return RangeSet {_Spec::ranges(this->_libObjPtr())};
    }
};

using ConstVariantWithUnsignedIntegerSelectorFieldClassOption =
    ConstVariantWithIntegerSelectorFieldClassOption<
        const bt_field_class_variant_with_selector_field_integer_unsigned_option>;

using ConstVariantWithSignedIntegerSelectorFieldClassOption =
    ConstVariantWithIntegerSelectorFieldClassOption<
        const bt_field_class_variant_with_selector_field_integer_signed_option>;

namespace internal {

template <typename LibObjT>
struct CommonVariantFieldClassSpec;

// Functions specific to mutable variant field classes
template <>
struct CommonVariantFieldClassSpec<bt_field_class> final
{
    static bt_field_class_variant_option *optionByIndex(bt_field_class * const libObjPtr,
                                                        const std::uint64_t index) noexcept
    {
        return bt_field_class_variant_borrow_option_by_index(libObjPtr, index);
    }

    static bt_field_class_variant_option *optionByName(bt_field_class * const libObjPtr,
                                                       const char * const name) noexcept
    {
        return bt_field_class_variant_borrow_option_by_name(libObjPtr, name);
    }
};

// Functions specific to constant variant field classes
template <>
struct CommonVariantFieldClassSpec<const bt_field_class> final
{
    static const bt_field_class_variant_option *
    optionByIndex(const bt_field_class * const libObjPtr, const std::uint64_t index) noexcept
    {
        return bt_field_class_variant_borrow_option_by_index_const(libObjPtr, index);
    }

    static const bt_field_class_variant_option *optionByName(const bt_field_class * const libObjPtr,
                                                             const char * const name) noexcept
    {
        return bt_field_class_variant_borrow_option_by_name_const(libObjPtr, name);
    }
};

} // namespace internal

template <typename LibObjT>
class CommonVariantFieldClass : public CommonFieldClass<LibObjT>
{
private:
    using typename CommonFieldClass<LibObjT>::_ThisCommonFieldClass;

protected:
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;
    using _ThisCommonVariantFieldClass = CommonVariantFieldClass<LibObjT>;

public:
    using Shared = internal::SharedFieldClass<CommonVariantFieldClass<LibObjT>, LibObjT>;

    using Option =
        typename std::conditional<std::is_const<LibObjT>::value, ConstVariantFieldClassOption,
                                  VariantFieldClassOption>::type;

    explicit CommonVariantFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isVariant());
    }

    template <typename OtherLibObjT>
    CommonVariantFieldClass(const CommonVariantFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonVariantFieldClass&
    operator=(const CommonVariantFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonFieldClass::operator=(fc);
        return *this;
    }

    std::uint64_t size() const noexcept
    {
        return bt_field_class_variant_get_option_count(this->_libObjPtr());
    }

    ConstVariantFieldClassOption operator[](const std::uint64_t index) const noexcept
    {
        return ConstVariantFieldClassOption {
            internal::CommonVariantFieldClassSpec<const bt_field_class>::optionByIndex(
                this->_libObjPtr(), index)};
    }

    Option operator[](const std::uint64_t index) noexcept
    {
        return Option {internal::CommonVariantFieldClassSpec<LibObjT>::optionByIndex(
            this->_libObjPtr(), index)};
    }

    nonstd::optional<ConstVariantFieldClassOption>
    operator[](const char * const name) const noexcept
    {
        const auto libObjPtr =
            internal::CommonVariantFieldClassSpec<const bt_field_class>::optionByName(
                this->_libObjPtr(), name);

        if (libObjPtr) {
            return ConstVariantFieldClassOption {libObjPtr};
        }

        return nonstd::nullopt;
    }

    nonstd::optional<ConstVariantFieldClassOption>
    operator[](const std::string& name) const noexcept
    {
        return (*this)[name.data()];
    }

    nonstd::optional<Option> operator[](const char * const name) noexcept
    {
        const auto libObjPtr =
            internal::CommonVariantFieldClassSpec<LibObjT>::optionByName(this->_libObjPtr(), name);

        if (libObjPtr) {
            return Option {libObjPtr};
        }

        return nonstd::nullopt;
    }

    nonstd::optional<Option> operator[](const std::string& name) noexcept
    {
        return (*this)[name.data()];
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using VariantFieldClass = CommonVariantFieldClass<bt_field_class>;
using ConstVariantFieldClass = CommonVariantFieldClass<const bt_field_class>;

template <typename LibObjT>
class CommonVariantWithoutSelectorFieldClass : public CommonVariantFieldClass<LibObjT>
{
private:
    using typename CommonVariantFieldClass<LibObjT>::_ThisCommonVariantFieldClass;
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;

public:
    using Shared =
        internal::SharedFieldClass<CommonVariantWithoutSelectorFieldClass<LibObjT>, LibObjT>;

    explicit CommonVariantWithoutSelectorFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonVariantFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isVariantWithoutSelector());
    }

    template <typename OtherLibObjT>
    CommonVariantWithoutSelectorFieldClass(
        const CommonVariantWithoutSelectorFieldClass<OtherLibObjT>& fc) noexcept :
        _ThisCommonVariantFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    CommonVariantWithoutSelectorFieldClass<LibObjT>&
    operator=(const CommonVariantWithoutSelectorFieldClass<OtherLibObjT>& fc) noexcept
    {
        _ThisCommonVariantFieldClass::operator=(fc);
        return *this;
    }

    void appendOption(const char * const name, const FieldClass& fc)
    {
        static_assert(!std::is_const<LibObjT>::value, "`LibObjT` must NOT be `const`.");

        const auto status = bt_field_class_variant_without_selector_append_option(
            this->_libObjPtr(), name, fc._libObjPtr());

        if (status ==
            BT_FIELD_CLASS_VARIANT_WITHOUT_SELECTOR_FIELD_APPEND_OPTION_STATUS_MEMORY_ERROR) {
            throw LibMemoryError {};
        }
    }

    void appendOption(const std::string& name, const FieldClass& fc)
    {
        this->appendOption(name.data(), fc);
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using VariantWithoutSelectorFieldClass = CommonVariantWithoutSelectorFieldClass<bt_field_class>;
using ConstVariantWithoutSelectorFieldClass =
    CommonVariantWithoutSelectorFieldClass<const bt_field_class>;

namespace internal {

template <typename OptionT>
struct CommonVariantWithIntegerSelectorFieldClassSpec;

// Functions specific to variant field classes with unsigned integer selector
template <>
struct CommonVariantWithIntegerSelectorFieldClassSpec<
    ConstVariantWithUnsignedIntegerSelectorFieldClassOption>
    final
{
    static const bt_field_class_variant_with_selector_field_integer_unsigned_option *
    optionByIndex(const bt_field_class * const libObjPtr, const std::uint64_t index) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_unsigned_borrow_option_by_index_const(
            libObjPtr, index);
    }

    static const bt_field_class_variant_with_selector_field_integer_unsigned_option *
    optionByName(const bt_field_class * const libObjPtr, const char * const name) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_unsigned_borrow_option_by_name_const(
            libObjPtr, name);
    }

    static bt_field_class_variant_with_selector_field_integer_append_option_status
    appendOption(bt_field_class * const libObjPtr, const char * const name,
                 bt_field_class * const libOptFcPtr,
                 const bt_integer_range_set_unsigned * const libRangesPtr)
    {
        return bt_field_class_variant_with_selector_field_integer_unsigned_append_option(
            libObjPtr, name, libOptFcPtr, libRangesPtr);
    }
};

// Functions specific to variant field classes with signed integer selector
template <>
struct CommonVariantWithIntegerSelectorFieldClassSpec<
    ConstVariantWithSignedIntegerSelectorFieldClassOption>
    final
{
    static const bt_field_class_variant_with_selector_field_integer_signed_option *
    optionByIndex(const bt_field_class * const libObjPtr, const std::uint64_t index) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_signed_borrow_option_by_index_const(
            libObjPtr, index);
    }

    static const bt_field_class_variant_with_selector_field_integer_signed_option *
    optionByName(const bt_field_class * const libObjPtr, const char * const name) noexcept
    {
        return bt_field_class_variant_with_selector_field_integer_signed_borrow_option_by_name_const(
            libObjPtr, name);
    }

    static bt_field_class_variant_with_selector_field_integer_append_option_status
    appendOption(bt_field_class * const libObjPtr, const char * const name,
                 bt_field_class * const libOptFcPtr,
                 const bt_integer_range_set_signed * const libRangesPtr)
    {
        return bt_field_class_variant_with_selector_field_integer_signed_append_option(
            libObjPtr, name, libOptFcPtr, libRangesPtr);
    }
};

} // namespace internal

template <typename LibObjT, typename OptionT>
class CommonVariantWithIntegerSelectorFieldClass : public CommonVariantFieldClass<LibObjT>
{
private:
    using typename CommonVariantFieldClass<LibObjT>::_ThisCommonVariantFieldClass;
    using typename CommonFieldClass<LibObjT>::_LibObjPtr;

    using _ThisCommonVariantWithIntegerSelectorFieldClass =
        CommonVariantWithIntegerSelectorFieldClass<LibObjT, OptionT>;

    using _Spec = internal::CommonVariantWithIntegerSelectorFieldClassSpec<OptionT>;

public:
    using Shared =
        internal::SharedFieldClass<_ThisCommonVariantWithIntegerSelectorFieldClass, LibObjT>;

    using Option = OptionT;

    explicit CommonVariantWithIntegerSelectorFieldClass(const _LibObjPtr libObjPtr) noexcept :
        _ThisCommonVariantFieldClass {libObjPtr}
    {
        BT_ASSERT_DBG(this->isVariant());
    }

    template <typename OtherLibObjT>
    CommonVariantWithIntegerSelectorFieldClass(
        const CommonVariantWithIntegerSelectorFieldClass<OtherLibObjT, OptionT>& fc) noexcept :
        _ThisCommonVariantFieldClass {fc}
    {
    }

    template <typename OtherLibObjT>
    _ThisCommonVariantWithIntegerSelectorFieldClass&
    operator=(const CommonVariantWithIntegerSelectorFieldClass<OtherLibObjT, OptionT>& fc) noexcept
    {
        _ThisCommonVariantFieldClass::operator=(fc);
        return *this;
    }

    ConstFieldPath selectorFieldPath() const noexcept
    {
        return ConstFieldPath {
            bt_field_class_option_with_selector_field_borrow_selector_field_path_const(
                this->_libObjPtr())};
    }

    Option operator[](const std::uint64_t index) const noexcept
    {
        return Option {_Spec::optionByIndex(this->_libObjPtr(), index)};
    }

    nonstd::optional<Option> operator[](const char * const name) const noexcept
    {
        const auto libObjPtr = _Spec::optionByName(this->_libObjPtr(), name);

        if (libObjPtr) {
            return Option {libObjPtr};
        }

        return nonstd::nullopt;
    }

    nonstd::optional<Option> operator[](const std::string& name) const noexcept
    {
        return (*this)[name.data()];
    }

    void appendOption(const char * const name, const FieldClass& fc,
                      const typename Option::RangeSet& ranges)
    {
        static_assert(!std::is_const<LibObjT>::value, "`LibObjT` must NOT be `const`.");

        const auto status =
            _Spec::appendOption(this->_libObjPtr(), name, fc._libObjPtr(), ranges._libObjPtr());

        if (status ==
            BT_FIELD_CLASS_VARIANT_WITH_SELECTOR_FIELD_APPEND_OPTION_STATUS_MEMORY_ERROR) {
            throw LibMemoryError {};
        }
    }

    void appendOption(const std::string& name, const FieldClass& fc)
    {
        this->appendOption(name.data(), fc);
    }

    Shared shared() const noexcept
    {
        return Shared {*this};
    }
};

using VariantWithUnsignedIntegerSelectorFieldClass = CommonVariantWithIntegerSelectorFieldClass<
    bt_field_class, ConstVariantWithUnsignedIntegerSelectorFieldClassOption>;

using ConstVariantWithUnsignedIntegerSelectorFieldClass =
    CommonVariantWithIntegerSelectorFieldClass<
        const bt_field_class, ConstVariantWithUnsignedIntegerSelectorFieldClassOption>;

using VariantWithSignedIntegerSelectorFieldClass = CommonVariantWithIntegerSelectorFieldClass<
    bt_field_class, ConstVariantWithSignedIntegerSelectorFieldClassOption>;

using ConstVariantWithSignedIntegerSelectorFieldClass = CommonVariantWithIntegerSelectorFieldClass<
    const bt_field_class, ConstVariantWithSignedIntegerSelectorFieldClassOption>;

template <typename LibObjT>
CommonBitArrayFieldClass<LibObjT> CommonFieldClass<LibObjT>::asBitArray() const noexcept
{
    BT_ASSERT_DBG(this->isBitArray());
    return CommonBitArrayFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonIntegerFieldClass<LibObjT> CommonFieldClass<LibObjT>::asInteger() const noexcept
{
    BT_ASSERT_DBG(this->isInteger());
    return CommonIntegerFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonEnumerationFieldClass<LibObjT, ConstUnsignedEnumerationFieldClassMapping>
CommonFieldClass<LibObjT>::asUnsignedEnumeration() const noexcept
{
    BT_ASSERT_DBG(this->isUnsignedEnumeration());
    return CommonEnumerationFieldClass<LibObjT, ConstUnsignedEnumerationFieldClassMapping> {
        this->_libObjPtr()};
}

template <typename LibObjT>
CommonEnumerationFieldClass<LibObjT, ConstSignedEnumerationFieldClassMapping>
CommonFieldClass<LibObjT>::asSignedEnumeration() const noexcept
{
    BT_ASSERT_DBG(this->isSignedEnumeration());
    return CommonEnumerationFieldClass<LibObjT, ConstSignedEnumerationFieldClassMapping> {
        this->_libObjPtr()};
}

template <typename LibObjT>
CommonStructureFieldClass<LibObjT> CommonFieldClass<LibObjT>::asStructure() const noexcept
{
    BT_ASSERT_DBG(this->isStructure());
    return CommonStructureFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonArrayFieldClass<LibObjT> CommonFieldClass<LibObjT>::asArray() const noexcept
{
    BT_ASSERT_DBG(this->isArray());
    return CommonArrayFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonStaticArrayFieldClass<LibObjT> CommonFieldClass<LibObjT>::asStaticArray() const noexcept
{
    BT_ASSERT_DBG(this->isStaticArray());
    return CommonStaticArrayFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonDynamicArrayWithLengthFieldClass<LibObjT>
CommonFieldClass<LibObjT>::asDynamicArrayWithLength() const noexcept
{
    BT_ASSERT_DBG(this->isDynamicArrayWithLength());
    return CommonDynamicArrayWithLengthFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonOptionFieldClass<LibObjT> CommonFieldClass<LibObjT>::asOption() const noexcept
{
    BT_ASSERT_DBG(this->isOption());
    return CommonOptionFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonOptionWithSelectorFieldClass<LibObjT>
CommonFieldClass<LibObjT>::asOptionWithSelector() const noexcept
{
    BT_ASSERT_DBG(this->isOptionWithSelector());
    return CommonOptionWithSelectorFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonOptionWithBoolSelectorFieldClass<LibObjT>
CommonFieldClass<LibObjT>::asOptionWithBoolSelector() const noexcept
{
    BT_ASSERT_DBG(this->isOptionWithBoolSelector());
    return CommonOptionWithBoolSelectorFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonOptionWithIntegerSelectorFieldClass<LibObjT, ConstUnsignedIntegerRangeSet>
CommonFieldClass<LibObjT>::asOptionWithUnsignedIntegerSelector() const noexcept
{
    BT_ASSERT_DBG(this->isOptionWithUnsignedIntegerSelector());
    return CommonOptionWithIntegerSelectorFieldClass<LibObjT, ConstUnsignedIntegerRangeSet> {
        this->_libObjPtr()};
}

template <typename LibObjT>
CommonOptionWithIntegerSelectorFieldClass<LibObjT, ConstSignedIntegerRangeSet>
CommonFieldClass<LibObjT>::asOptionWithSignedIntegerSelector() const noexcept
{
    BT_ASSERT_DBG(this->isOptionWithSignedIntegerSelector());
    return CommonOptionWithIntegerSelectorFieldClass<LibObjT, ConstSignedIntegerRangeSet> {
        this->_libObjPtr()};
}

template <typename LibObjT>
CommonVariantFieldClass<LibObjT> CommonFieldClass<LibObjT>::asVariant() const noexcept
{
    BT_ASSERT_DBG(this->isVariant());
    return CommonVariantFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonVariantWithoutSelectorFieldClass<LibObjT>
CommonFieldClass<LibObjT>::asVariantWithoutSelector() const noexcept
{
    BT_ASSERT_DBG(this->isVariantWithoutSelector());
    return CommonVariantWithoutSelectorFieldClass<LibObjT> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonVariantWithIntegerSelectorFieldClass<LibObjT,
                                           ConstVariantWithUnsignedIntegerSelectorFieldClassOption>
CommonFieldClass<LibObjT>::asVariantWithUnsignedIntegerSelector() const noexcept
{
    BT_ASSERT_DBG(this->isVariantWithUnsignedIntegerSelector());
    return CommonVariantWithIntegerSelectorFieldClass<
        LibObjT, ConstVariantWithUnsignedIntegerSelectorFieldClassOption> {this->_libObjPtr()};
}

template <typename LibObjT>
CommonVariantWithIntegerSelectorFieldClass<LibObjT,
                                           ConstVariantWithSignedIntegerSelectorFieldClassOption>
CommonFieldClass<LibObjT>::asVariantWithSignedIntegerSelector() const noexcept
{
    BT_ASSERT_DBG(this->isVariantWithSignedIntegerSelector());
    return CommonVariantWithIntegerSelectorFieldClass<
        LibObjT, ConstVariantWithSignedIntegerSelectorFieldClassOption> {this->_libObjPtr()};
}

} // namespace bt2

#endif // BABELTRACE_CPP_COMMON_BT2_FIELD_CLASS_HPP
