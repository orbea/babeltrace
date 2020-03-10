# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2019 EfficiOS Inc.
#

import bt2
import unittest


class PackageTestCase(unittest.TestCase):
    def _assert_in_bt2(self, name):
        self.assertIn(name, dir(bt2))


_public_names = [
    '__version__',
    '_ArrayField',
    '_ArrayFieldClass',
    '_ArrayFieldClassConst',
    '_ArrayFieldConst',
    '_ArrayValueConst',
    '_BitArrayField',
    '_BitArrayFieldClass',
    '_BitArrayFieldClassConst',
    '_BitArrayFieldConst',
    '_BoolField',
    '_BoolFieldClass',
    '_BoolFieldClassConst',
    '_BoolFieldConst',
    '_BoolValueConst',
    '_ClockSnapshotConst',
    '_ComponentClassErrorCause',
    '_ComponentErrorCause',
    '_CurrentArrayElementFieldPathItem',
    '_CurrentOptionContentFieldPathItem',
    '_DiscardedEventsMessage',
    '_DiscardedEventsMessageConst',
    '_DiscardedPacketsMessage',
    '_DiscardedPacketsMessageConst',
    '_DoublePrecisionRealField',
    '_DoublePrecisionRealFieldConst',
    '_DynamicArrayField',
    '_DynamicArrayFieldClass',
    '_DynamicArrayFieldClassConst',
    '_DynamicArrayFieldConst',
    '_DynamicArrayWithLengthFieldFieldClass',
    '_DynamicArrayWithLengthFieldFieldClassConst',
    '_EnumerationField',
    '_EnumerationFieldClass',
    '_EnumerationFieldClassConst',
    '_EnumerationFieldConst',
    '_Error',
    '_ErrorCause',
    '_EventMessage',
    '_EventMessageConst',
    '_FilterComponentClassConst',
    '_FilterComponentConst',
    '_IndexFieldPathItem',
    '_IntegerField',
    '_IntegerFieldClass',
    '_IntegerFieldClassConst',
    '_IntegerFieldConst',
    '_IntegerValue',
    '_IntegerValueConst',
    '_MapValueConst',
    '_MessageIteratorErrorCause',
    '_MessageIteratorInactivityMessage',
    '_MessageIteratorInactivityMessageConst',
    '_OptionFieldClass',
    '_OptionFieldClassConst',
    '_OptionWithBoolSelectorFieldClass',
    '_OptionWithBoolSelectorFieldClassConst',
    '_OptionWithIntegerSelectorFieldClass',
    '_OptionWithIntegerSelectorFieldClassConst',
    '_OptionWithSelectorFieldClass',
    '_OptionWithSelectorFieldClassConst',
    '_OptionWithSignedIntegerSelectorFieldClass',
    '_OptionWithSignedIntegerSelectorFieldClassConst',
    '_OptionWithUnsignedIntegerSelectorFieldClass',
    '_OptionWithUnsignedIntegerSelectorFieldClassConst',
    '_PacketBeginningMessage',
    '_PacketBeginningMessageConst',
    '_PacketEndMessage',
    '_PacketEndMessageConst',
    '_RealField',
    '_RealFieldClass',
    '_RealFieldClassConst',
    '_RealFieldConst',
    '_RealValueConst',
    '_SignedEnumerationField',
    '_SignedEnumerationFieldClass',
    '_SignedEnumerationFieldClassConst',
    '_SignedEnumerationFieldConst',
    '_SignedIntegerField',
    '_SignedIntegerFieldClass',
    '_SignedIntegerFieldClassConst',
    '_SignedIntegerFieldConst',
    '_SignedIntegerRangeConst',
    '_SignedIntegerRangeSetConst',
    '_SignedIntegerValueConst',
    '_SinglePrecisionRealField',
    '_SinglePrecisionRealFieldConst',
    '_SinkComponentClassConst',
    '_SinkComponentConst',
    '_SourceComponentClassConst',
    '_SourceComponentConst',
    '_StaticArrayField',
    '_StaticArrayFieldClass',
    '_StaticArrayFieldClassConst',
    '_StaticArrayFieldConst',
    '_StreamBeginningMessage',
    '_StreamBeginningMessageConst',
    '_StreamEndMessage',
    '_StreamEndMessageConst',
    '_StringField',
    '_StringFieldClass',
    '_StringFieldClassConst',
    '_StringFieldConst',
    '_StringValueConst',
    '_StructureField',
    '_StructureFieldClass',
    '_StructureFieldClassConst',
    '_StructureFieldConst',
    '_UnknownClockSnapshot',
    '_UnsignedEnumerationField',
    '_UnsignedEnumerationFieldClass',
    '_UnsignedEnumerationFieldClassConst',
    '_UnsignedEnumerationFieldConst',
    '_UnsignedIntegerField',
    '_UnsignedIntegerFieldClass',
    '_UnsignedIntegerFieldClassConst',
    '_UnsignedIntegerFieldConst',
    '_UnsignedIntegerRangeConst',
    '_UnsignedIntegerRangeSetConst',
    '_UnsignedIntegerValueConst',
    '_UserFilterComponent',
    '_UserMessageIterator',
    '_UserSinkComponent',
    '_UserSourceComponent',
    '_VariantField',
    '_VariantField',
    '_VariantFieldClass',
    '_VariantFieldClassConst',
    '_VariantFieldClassWithIntegerSelector',
    '_VariantFieldClassWithIntegerSelectorConst',
    '_VariantFieldClassWithoutSelector',
    '_VariantFieldClassWithoutSelectorConst',
    '_VariantFieldClassWithSignedIntegerSelector',
    '_VariantFieldClassWithSignedIntegerSelectorConst',
    '_VariantFieldClassWithUnsignedIntegerSelector',
    '_VariantFieldClassWithUnsignedIntegerSelectorConst',
    '_VariantFieldConst',
    '_VariantFieldConst',
    'ArrayValue',
    'AutoSourceComponentSpec',
    'BoolValue',
    'ClockClassOffset',
    'ComponentClassType',
    'ComponentDescriptor',
    'ComponentSpec',
    'create_value',
    'EventClassLogLevel',
    'FieldPathScope',
    'find_plugin',
    'find_plugins',
    'find_plugins_in_path',
    'get_global_logging_level',
    'get_minimal_logging_level',
    'Graph',
    'IntegerDisplayBase',
    'Interrupter',
    'LoggingLevel',
    'MapValue',
    'plugin_component_class',
    'QueryExecutor',
    'RealValue',
    'register_plugin',
    'set_global_logging_level',
    'SignedIntegerRange',
    'SignedIntegerRangeSet',
    'SignedIntegerValue',
    'StringValue',
    'TraceCollectionMessageIterator',
    'UnsignedIntegerRange',
    'UnsignedIntegerRangeSet',
    'UnsignedIntegerValue',
]

for name in _public_names:

    def test_func(self):
        self._assert_in_bt2(name)

    setattr(PackageTestCase, 'test_has_' + name, test_func)


if __name__ == '__main__':
    unittest.main()
