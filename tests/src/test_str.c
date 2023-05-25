#include <assert.h>
#include <stdint.h>

#include "../../inc/vita/container/str.h"

void print_str(void *ptr, size_t i);
void apply_func(char *c, size_t i);

int32_t main(void) {
    VT_DEBUG_DEFAULT_INIT();

    const vt_str_t static_s = vt_str_make_on_stack("hello, world");
    assert(vt_str_equals(vt_cstr(&static_s), "hello, world"));
    assert(vt_str_starts_with(vt_cstr(&static_s), "hello"));
    assert(vt_str_ends_with(vt_cstr(&static_s), "world"));

    vt_str_t* mystr = vt_str("hello, world!"); {
        assert(vt_str_len(mystr) == 13);
        assert(vt_str_capacity(mystr) == 13);
        assert(vt_str_has_space(mystr) == 0);
        assert(!vt_str_is_empty(mystr));
        assert(vt_str_equals(vt_cstr(mystr), "hello, world!"));

        vt_str_reserve(mystr, 65);
        assert(vt_str_len(mystr) == 13);
        assert(vt_str_capacity(mystr) == 65 + 13);
        assert(vt_str_has_space(mystr) == 65);

        vt_str_set(mystr, "Apples are sweet!");
        assert(vt_str_len(mystr) == 17);
        assert(vt_str_capacity(mystr) == 65 + 13);
        assert(vt_str_has_space(mystr) == 65 + 13 - 17);
        assert(vt_str_equals(vt_cstr(mystr), "Apples are sweet!"));

        vt_str_append(mystr, "Watermellon is red?");
        assert(vt_str_len(mystr) == 17 + 19);
        assert(vt_str_capacity(mystr) == 65 + 13);
        assert(vt_str_has_space(mystr) == 65 + 13 - 17 - 19);

        vt_str_insert(mystr, "Oranges.", 0);
        assert(vt_str_len(mystr) == 17 + 19 + 8);
        assert(vt_str_capacity(mystr) == 65 + 13);
        assert(vt_str_has_space(mystr) == 65 + 13 - 17 - 19 - 8);

        vt_str_remove(mystr, 4, 3);
        vt_str_remove_first(mystr, "Watermellon is red");
        assert(vt_str_can_find(mystr, "sweet") == 1);
        assert(vt_str_equals(vt_cstr(mystr), "Oran.Apples are sweet!?"));

        const vt_str_t sstack = vt_str_make_on_stack("hi this is a test hi, world hi");
        assert(vt_str_can_find(&sstack, "hi") == 3);

        vt_str_shrink(mystr);
        assert(vt_str_len(mystr) == 23);
        assert(vt_str_capacity(mystr) == 23);
        assert(vt_str_has_space(mystr) == 0);

        vt_str_t *copy = vt_str_dup(mystr); {
            assert(vt_str_len(copy) == 23);
            assert(vt_str_capacity(copy) == 23);
            assert(vt_str_has_space(copy) == 0);
        } vt_str_free(copy);

        vt_str_clear(mystr);
        assert(vt_str_len(mystr) == 0);
        assert(vt_str_capacity(mystr) == 23);
        assert(vt_str_has_space(mystr) == 23);
        assert(vt_str_equals(vt_cstr(mystr), ""));

        vt_str_t *copy2 = vt_strn(10); {
            assert(vt_str_len(copy2) == 10);
            assert(vt_str_capacity(copy2) == 10);
            assert(vt_str_has_space(copy2) == 0);
        } vt_str_free(copy2);
    } vt_str_free(mystr);

    vt_str_t *ns = vt_strn(1); {
        assert(vt_str_len(ns) == 1);
        assert(vt_str_capacity(ns) == 1);
        assert(vt_str_has_space(ns) == 0);

        assert(vt_str_append(ns, "hello") == vt_status_operation_success);
        assert(vt_str_equals(vt_cstr(ns), "hello"));

        // fails, because vt_str_len(ns) < strlen("hello, world") => append instead
        // assert(vt_str_set(ns, "hello, world"));
    } vt_str_free(ns);

    vt_str_t *sto = vt_str_take_ownership(strdup("12345, world")); {
        assert(vt_str_len(sto) == 12);
        assert(vt_str_capacity(sto) == 12);
        assert(vt_str_has_space(sto) == 0);

        assert(vt_str_append(sto, "! How are you?") == vt_status_operation_success);
        assert(vt_str_len(sto) == strlen("12345, world! How are you?"));
        assert(vt_str_equals(vt_cstr(sto), "12345, world! How are you?"));
        assert(vt_str_append(sto, " hello, world; hello again. This is hello!") == vt_status_operation_success);

        vt_str_clear(sto);
        vt_str_append(sto, ";My name is Kirillos;How are you?;let's play;");
        vt_plist_t *p = vt_str_split(NULL, sto, ";"); {
            assert(vt_plist_len(p) == 3);

            // free each vt_str_t in vt_plist_t
            vt_plist_apply(p, print_str);
        } vt_plist_destroy(p);

        vt_str_t *spf = vt_str_pop_get_first(NULL, sto, "Kirillos"); {
            assert(vt_str_equals(vt_cstr(spf), ";My name is "));
        } vt_str_free(spf);

        vt_str_t *spl = vt_str_pop_get_last(NULL, sto, "you?;"); {
            assert(vt_str_equals(vt_cstr(spl), "let's play;"));
        } vt_str_free(spl);

        assert(vt_str_equals(vt_cstr(sto), ";How are "));

        vt_str_t *_s__ = vt_str("hello world of world of wonders!"); {
            vt_str_t *_s_ = vt_str_pop_get_last(NULL, _s__, "world"); {
                assert(vt_str_equals(vt_cstr(_s_), " of wonders!"));
            } vt_str_free(_s_);
            
            assert(vt_str_equals(vt_cstr(_s__), "hello world of "));
        } vt_str_free(_s__);

    } vt_str_free(sto);

    vt_str_t *s_strip = vt_str("   \nhello, world\t  ");
    vt_str_strip(s_strip);
    assert(vt_str_len(s_strip) == 12);
    assert(vt_str_equals(vt_cstr(s_strip), "hello, world"));
    vt_str_free(s_strip);

    s_strip = vt_str("\n  ..,!  hello, world  ;;-!  \t\n");
    vt_str_strip_punct(s_strip);
    assert(vt_str_len(s_strip) == 12);
    assert(vt_str_equals(vt_cstr(s_strip), "hello, world"));
    vt_str_free(s_strip);

    s_strip = vt_str("\n  ..,!  hello, world  ;;-!  \t\n");
    vt_str_strip_c(s_strip, "\n .,;!\t-");
    assert(vt_str_len(s_strip) == 12);
    assert(vt_str_equals(vt_cstr(s_strip), "hello, world"));
    vt_str_free(s_strip);

    s_strip = vt_str("\n  ..,!  hello, world  ;;-!  \t\n");
    vt_str_remove_c(s_strip, "\n .,;!\t-");
    assert(vt_str_len(s_strip) == 10);
    assert(vt_str_equals(vt_cstr(s_strip), "helloworld"));
    vt_str_free(s_strip);
    
    s_strip = vt_str("Here is a shopping list: apples and oranges, milk and sugar, and vinegar.");
    vt_str_remove_all(s_strip, " and");
    assert(vt_str_equals(vt_cstr(s_strip), "Here is a shopping list: apples oranges, milk sugar, vinegar."));
    vt_str_free(s_strip);

    s_strip = vt_str("hello, world, world!");
    vt_str_remove_last(s_strip, ", world");
    assert(vt_str_equals(vt_cstr(s_strip), "hello, world!"));
    assert(vt_str_len(s_strip) == 13);
    vt_str_free(s_strip);

    s_strip = vt_str("A A A");
    vt_str_apply(s_strip, apply_func);
    assert(vt_str_equals(vt_cstr(s_strip), "bbbbb"));
    vt_str_clear(s_strip);

    vt_str_appendf(s_strip, "%s %d %s", "I have", 2, "apples");
    assert(vt_str_equals(vt_cstr(s_strip), "I have 2 apples"));
    vt_str_free(s_strip);

    s_strip = vt_str_fmt(NULL, "%s, %s. %s %d!", "hello", "world", "hi", 5);
    assert(vt_str_equals(vt_cstr(s_strip), "hello, world. hi 5!"));
    vt_str_free(s_strip);

    s_strip = vt_strn(1);
    vt_str_fmt(s_strip, "%s, %s. %s %d!", "hello", "world", "hi", 5);
    assert(vt_str_equals(vt_cstr(s_strip), "hello, world. hi 5!"));
    vt_str_free(s_strip);

    s_strip = vt_str("hello, world!");
    vt_str_capitalize(s_strip);
    assert(vt_str_equals(vt_cstr(s_strip), "HELLO, WORLD!"));
    assert(!vt_str_is_numeric(vt_cstr(s_strip), 256));
    
    vt_str_set(s_strip, "123");
    assert(vt_str_is_numeric(vt_cstr(s_strip), 256));

    vt_str_set(s_strip, "564.025");
    assert(vt_str_is_numeric(vt_cstr(s_strip), 256));
    vt_str_free(s_strip);

    vt_str_t *sbetween = vt_str_split_between("https :1.8.8.8@ world", ":", "@");
    assert(vt_str_equals(vt_cstr(sbetween), "1.8.8.8"));
    vt_str_free(sbetween);

    vt_str_t *s_val_test = vt_str("123456789"); {
        assert(vt_str_len(s_val_test) == 9);
        assert(vt_str_capacity(s_val_test) == 9);

        ((char*)s_val_test->ptr)[4] = '\0';
        assert(vt_str_validate_len(s_val_test) == 4);

        assert(vt_str_len(s_val_test) == 4);
        assert(vt_str_capacity(s_val_test) == 9);

    } vt_str_free(s_val_test);

    VT_DEBUG_DEFAULT_QUIT();
    return 0;
}

void print_str(void *ptr, size_t i) {
    vt_str_free(ptr);
}

void apply_func(char *c, size_t i) {
    *c = 'b';
}
