#ifndef ATTRIBUTETYPE_H
#define ATTRIBUTETYPE_H

namespace PersistenceFramework
{
	enum AttributeType
	{
		_invalid = -1,
		_int = 1,
		_float = 2,
		_double = 3,
		_enum = 4,
		_long = 5,
		_string = 6,
		_varchar = 7,
		_char = 8,
		_bool = 9,
		_date = 10,
		_object = 11,
		_variantValue = 12,
		_variantType = 13,
		_list_1_N = 14,
		_list_N_N = 15
	};
}

#endif