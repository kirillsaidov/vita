#include <assert.h>

#include "../../inc/vita/container/str.h"

void print_str(void *ptr, size_t i);

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
		assert(str_can_find(mystr, "sweet") == 1);
		assert(str_equals(cstr(mystr), "Oran.Apples are sweet!?"));

		str_shrink(mystr);
		assert(str_len(mystr) == 23);
		assert(str_capacity(mystr) == 23);
		assert(str_has_space(mystr) == 0);

		str_t *copy = str_dup(mystr); {
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

		// fails, because str_len(ns) < strlen("hello, world") => append instead
		// assert(str_set(ns, "hello, world"));
	} str_free(ns);

	str_t *sto = str_take_ownership(strdup("12345, world")); {
		assert(str_len(sto) == 12);
		assert(str_capacity(sto) == 12);
		assert(str_has_space(sto) == 0);

		assert(str_append(sto, "! How are you?"));
		assert(str_len(sto) == strlen("12345, world! How are you?"));
		assert(str_equals(cstr(sto), "12345, world! How are you?"));
		assert(str_append(sto, " hello, world; hello again. This is hello!"));

		str_clear(sto);
		str_append(sto, ";My name is Kirillos;How are you?;let's play;");
		plist_t *p = str_split(NULL, sto, ";"); {
			assert(plist_len(p) == 3);

			// free each str_t in plist_t
			plist_foreach(p, print_str);
		} plist_destroy(p);
	} str_free(sto);

	return 0;
}

void print_str(void *ptr, size_t i) {
	str_free(ptr);
}
