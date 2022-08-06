#include <assert.h>
#include <stdint.h>

#include "../../inc/vita/container/str.h"

void print_str(void *ptr, size_t i);
void apply_func(char *c, size_t i);

int32_t main(void) {
    const str_t static_s = str_make_on_stack("hello, world");
    assert(str_equals(cstr(&static_s), "hello, world"));
    assert(str_starts_with(cstr(&static_s), "hello"));
    assert(str_ends_with(cstr(&static_s), "world"));

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
        str_remove_first(mystr, "Watermellon is red");
        assert(str_can_find(mystr, "sweet") == 1);
        assert(str_equals(cstr(mystr), "Oran.Apples are sweet!?"));

        const str_t sstack = str_make_on_stack("hi this is a test hi, world hi");
        assert(str_can_find(&sstack, "hi") == 3);

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

        assert(str_append(ns, "hello") == ve_operation_success);
        assert(str_equals(cstr(ns), "hello"));

        // fails, because str_len(ns) < strlen("hello, world") => append instead
        // assert(str_set(ns, "hello, world"));
    } str_free(ns);

    str_t *sto = str_take_ownership(strdup("12345, world")); {
        assert(str_len(sto) == 12);
        assert(str_capacity(sto) == 12);
        assert(str_has_space(sto) == 0);

        assert(str_append(sto, "! How are you?") == ve_operation_success);
        assert(str_len(sto) == strlen("12345, world! How are you?"));
        assert(str_equals(cstr(sto), "12345, world! How are you?"));
        assert(str_append(sto, " hello, world; hello again. This is hello!") == ve_operation_success);

        str_clear(sto);
        str_append(sto, ";My name is Kirillos;How are you?;let's play;");
        plist_t *p = str_split(NULL, sto, ";"); {
            assert(plist_len(p) == 3);

            // free each str_t in plist_t
            plist_apply(p, print_str);
        } plist_destroy(p);

        str_t *spf = str_pop_get_first(NULL, sto, "Kirillos"); {
            assert(str_equals(cstr(spf), ";My name is "));
        } str_free(spf);

        str_t *spl = str_pop_get_last(NULL, sto, "you?;"); {
            assert(str_equals(cstr(spl), "let's play;"));
        } str_free(spl);

        assert(str_equals(cstr(sto), ";How are "));

        str_t *_s__ = str("hello world of world of wonders!"); {
            str_t *_s_ = str_pop_get_last(NULL, _s__, "world"); {
                assert(str_equals(cstr(_s_), " of wonders!"));
            } str_free(_s_);
            
            assert(str_equals(cstr(_s__), "hello world of "));
        } str_free(_s__);

    } str_free(sto);

    str_t *s_strip = str("   \nhello, world\t  ");
    str_strip(s_strip);
    assert(str_len(s_strip) == 12);
    assert(str_equals(cstr(s_strip), "hello, world"));
    str_free(s_strip);

    s_strip = str("\n  ..,!  hello, world  ;;-!  \t\n");
    str_strip_punct(s_strip);
    assert(str_len(s_strip) == 12);
    assert(str_equals(cstr(s_strip), "hello, world"));
    str_free(s_strip);

    s_strip = str("\n  ..,!  hello, world  ;;-!  \t\n");
    str_strip_c(s_strip, "\n .,;!\t-");
    assert(str_len(s_strip) == 12);
    assert(str_equals(cstr(s_strip), "hello, world"));
    str_free(s_strip);

    s_strip = str("\n  ..,!  hello, world  ;;-!  \t\n");
    str_remove_c(s_strip, "\n .,;!\t-");
    assert(str_len(s_strip) == 10);
    assert(str_equals(cstr(s_strip), "helloworld"));
    str_free(s_strip);
    
    s_strip = str("Here is a shopping list: apples and oranges, milk and sugar, and vinegar.");
    str_remove_all(s_strip, " and");
    assert(str_equals(cstr(s_strip), "Here is a shopping list: apples oranges, milk sugar, vinegar."));
    str_free(s_strip);

    s_strip = str("hello, world, world!");
    str_remove_last(s_strip, ", world");
    assert(str_equals(cstr(s_strip), "hello, world!"));
    assert(str_len(s_strip) == 13);
    str_free(s_strip);

    s_strip = str("A A A");
    str_apply(s_strip, apply_func);
    assert(str_equals(cstr(s_strip), "bbbbb"));
    str_clear(s_strip);

    str_appendf(s_strip, "%s %d %s", "I have", 2, "apples");
    assert(str_equals(cstr(s_strip), "I have 2 apples"));
    str_free(s_strip);

    s_strip = str_fmt(NULL, "%s, %s. %s %d!", "hello", "world", "hi", 5);
    assert(str_equals(cstr(s_strip), "hello, world. hi 5!"));
    str_free(s_strip);

    s_strip = strn(0);
    str_fmt(s_strip, "%s, %s. %s %d!", "hello", "world", "hi", 5);
    assert(str_equals(cstr(s_strip), "hello, world. hi 5!"));
    str_free(s_strip);

    return 0;
}

void print_str(void *ptr, size_t i) {
    str_free(ptr);
}

void apply_func(char *c, size_t i) {
    *c = 'b';
}
