#include <assert.h>

#include "../../inc/vita/string/str.h"

int main(void) {
	str_t* mystr = str("hello, world!"); {
		assert(str_len(mystr) == 13);
		assert(str_capacity(mystr) == 13);
		assert(str_has_space(mystr) == 0);
		assert(!str_is_empty(mystr));
		assert(str_equals(cstr(mystr), "hello, world!"));

		str_reserve(mystr, 65);
		assert(str_len(mystr) == 13);
		assert(str_capacity(mystr) == 65 + 13);
		assert(str_has_space(mystr) == 65);

		str_set(mystr, "Apples are sweet!");
		assert(str_len(mystr) == 17);
		assert(str_capacity(mystr) == 65 + 13);
		assert(str_has_space(mystr) == 65 + 13 - 17);
		assert(str_equals(cstr(mystr), "Apples are sweet!"));
		
		str_append(mystr, "Watermellon is red?");
		assert(str_len(mystr) == 17 + 19);
		assert(str_capacity(mystr) == 65 + 13);
		assert(str_has_space(mystr) == 65 + 13 - 17 - 19);

		str_insert(mystr, "Oranges.", 0);
		assert(str_len(mystr) == 17 + 19 + 8);
		assert(str_capacity(mystr) == 65 + 13);
		assert(str_has_space(mystr) == 65 + 13 - 17 - 19 - 8);

		str_remove(mystr, 4, 3);
		str_remove_str(mystr, "Watermellon is red");
		assert(str_contains(mystr, "sweet") == 1);
		assert(str_equals(cstr(mystr), "Oran.Apples are sweet!?"));

		str_shrink(mystr);
		assert(str_len(mystr) == 23);
		assert(str_capacity(mystr) == 23);
		assert(str_has_space(mystr) == 0);

		str_t* copy = str_dup(mystr); {
			assert(str_len(copy) == 23);
			assert(str_capacity(copy) == 23);
			assert(str_has_space(copy) == 0);
		} str_free(copy);

		str_clear(mystr);
		assert(str_len(mystr) == 0);
		assert(str_capacity(mystr) == 23);
		assert(str_has_space(mystr) == 23);
		assert(str_equals(cstr(mystr), ""));

		str_t *copy2 = strn(10); {
			assert(str_len(copy2) == 10);
			assert(str_capacity(copy2) == 10);
			assert(str_has_space(copy2) == 0);
		} str_free(copy2);
	} str_free(mystr);

	str_t *ns = strn(0); {
		assert(str_len(ns) == 0);
		assert(str_capacity(ns) == 0);
		assert(str_has_space(ns) == 0);

		assert(str_append(ns, "hello"));
		assert(str_equals(cstr(ns), "hello"));
		// assert(str_set(ns, "hello, world")); // fails, str_len(ns) < strlen("hello, world")
	} str_free(ns);
	
	return 0;
}













