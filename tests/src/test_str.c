#include <assert.h>
#include <stdint.h>

#include "../../inc/vita/container/str.h"

void print_str(void *ptr, size_t i);
void apply_func(char *c, size_t i);

int32_t main(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    const vt_str_t static_s = vt_str_create_static("hello, world");
    assert(vt_str_equals(vt_str_z(&static_s), "hello, world"));
    assert(vt_str_starts_with(vt_str_z(&static_s), "hello"));
    assert(vt_str_ends_with(vt_str_z(&static_s), "world"));

    vt_str_t* mystr = vt_str_create("hello, world!", alloctr); {
        assert(vt_str_len(mystr) == 13);
        assert(vt_str_capacity(mystr) == 13);
        assert(vt_str_has_space(mystr) == 0);
        assert(!vt_str_is_empty(mystr));
        assert(vt_str_equals(vt_str_z(mystr), "hello, world!"));

        vt_str_reserve(mystr, 65);
        assert(vt_str_len(mystr) == 13);
        assert(vt_str_capacity(mystr) == 65 + 13);
        assert(vt_str_has_space(mystr) == 65);

        vt_str_set(mystr, "Apples are sweet!");
        assert(vt_str_len(mystr) == 17);
        assert(vt_str_capacity(mystr) == 65 + 13);
        assert(vt_str_has_space(mystr) == 65 + 13 - 17);
        assert(vt_str_equals(vt_str_z(mystr), "Apples are sweet!"));

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
        assert(vt_str_equals(vt_str_z(mystr), "Oran.Apples are sweet!?"));

        const vt_str_t sstack = vt_str_create_static("hi this is a test hi, world hi");
        assert(vt_str_can_find(&sstack, "hi") == 3);

        vt_str_shrink(mystr);
        assert(vt_str_len(mystr) == 23);
        assert(vt_str_capacity(mystr) == 23);
        assert(vt_str_has_space(mystr) == 0);

        vt_str_t *copy = vt_str_dup(mystr); {
            assert(vt_str_len(copy) == 23);
            assert(vt_str_capacity(copy) == 23);
            assert(vt_str_has_space(copy) == 0);
        } vt_str_destroy(copy);

        vt_str_clear(mystr);
        assert(vt_str_len(mystr) == 0);
        assert(vt_str_capacity(mystr) == 23);
        assert(vt_str_has_space(mystr) == 23);
        assert(vt_str_equals(vt_str_z(mystr), ""));

        vt_str_t *copy2 = vt_str_create_len(10, alloctr); {
            assert(vt_str_len(copy2) == 10);
            assert(vt_str_capacity(copy2) == 10);
            assert(vt_str_has_space(copy2) == 0);
        } vt_str_destroy(copy2);
    } vt_str_destroy(mystr);

    // check insertf, insert_n
    vt_str_t *sss = vt_str_create("hello ", alloctr); {
        vt_str_appendf(sss, "%s", "world");
        vt_str_append(sss, "!");
        // vt_str_insert(sss, ",", 5);
        vt_str_append(sss, " hello ");
        vt_str_appendf(sss, "%s", "world!");
        printf("%s | len: %zu\n", vt_str_z(sss), vt_str_len(sss));
        assert(vt_str_equals(vt_str_z(sss), "hello, world! hello world!"));
    } vt_str_destroy(sss);
    vt_str_t *tmp_str = vt_str_create("<defs></defs>", alloctr); {
        vt_str_insert(tmp_str, "[]", 6);
        printf("%s | len: %zu\n", vt_str_z(tmp_str), vt_str_len(tmp_str));
        assert(vt_str_equals(vt_str_z(tmp_str), "<defs>[]</defs>"));
        // vt_str_insert_n(tmp_str, "hello, world! HAHA", 6, 13);
        // printf("%s | len: %zu\n", vt_str_z(tmp_str), vt_str_len(tmp_str));
        // assert(vt_str_equals(vt_str_z(tmp_str), "<defs>hello, world!</defs>"));

        // vt_str_insert(tmp_str, "[", 6);
        // // vt_str_insert(tmp_str, "]", 20);
        // printf("%s | len: %zu\n", vt_str_z(tmp_str), vt_str_len(tmp_str));
        // assert(vt_str_equals(vt_str_z(tmp_str), "<defs>[hello, world!</defs>"));

        // vt_str_insertf(tmp_str, 6, "%s", "[][][]");
        // assert(vt_str_equals(vt_str_z(tmp_str), "<defs>[][][]hello, world!</defs>"));

        // vt_str_insert_n(tmp_str, "1234567890", 7, 3);
        // vt_str_insert_n(tmp_str, "4567890", 12, 3);
        // vt_str_insert_n(tmp_str, "7890", 17, 3);
        // assert(vt_str_equals(vt_str_z(tmp_str), "<defs>[123][456][789]hello, world!</defs>"));

        // vt_str_clear(tmp_str);
        // vt_str_append(tmp_str, "hello ");
        // vt_str_appendf(tmp_str, "world ");
        // vt_str_append(tmp_str, "APPEND");
        // assert(vt_str_equals(vt_str_z(tmp_str), "hello world APPEND"));
        // printf("----- [%s]\n", vt_str_z(tmp_str));
    } vt_str_destroy(tmp_str);
    
    vt_str_t *ns = vt_str_create_len(1, alloctr); {
        assert(vt_str_len(ns) == 1);
        assert(vt_str_capacity(ns) == 1);
        assert(vt_str_has_space(ns) == 0);

        vt_str_append(ns, "hello");
        printf("%s | len: %zu\n", vt_str_z(ns), vt_str_len(ns));
        assert(vt_str_equals(vt_str_z(ns), " hello"));

        // fails, because vt_str_len(ns) < strlen("hello, world") => append instead
        // assert(vt_str_set(ns, "hello, world"));
    } vt_str_destroy(ns);

    vt_str_t *sto = vt_str_take_ownership(strdup("12345, world"), alloctr); {
        assert(vt_str_len(sto) == 12);
        assert(vt_str_capacity(sto) == 12);
        assert(vt_str_has_space(sto) == 0);

        vt_str_append(sto, "! How are you?");
        assert(vt_str_len(sto) == strlen("12345, world! How are you?"));
        assert(vt_str_equals(vt_str_z(sto), "12345, world! How are you?"));
        vt_str_append(sto, " hello, world; hello again. This is hello!");

        vt_str_clear(sto);
        vt_str_append(sto, ";My name is Kirillos;How are you?;let's play;");
        vt_plist_t *p = vt_str_split(NULL, sto, ";"); {
            assert(vt_plist_len(p) == 3);

            // free each vt_str_t in vt_plist_t
            vt_plist_apply(p, print_str);
        } vt_plist_destroy(p);

        vt_str_t *spf = vt_str_pop_get_first(NULL, sto, "Kirillos"); {
            assert(vt_str_equals(vt_str_z(spf), ";My name is "));
        } vt_str_destroy(spf);

        vt_str_t *spl = vt_str_pop_get_last(NULL, sto, "you?;"); {
            assert(vt_str_equals(vt_str_z(spl), "let's play;"));
        } vt_str_destroy(spl);

        assert(vt_str_equals(vt_str_z(sto), ";How are "));
        vt_str_t *_s__ = vt_str_create("hello world of world of wonders!", alloctr); {
            vt_str_t *_s_ = vt_str_pop_get_last(NULL, _s__, "world"); {
                assert(vt_str_equals(vt_str_z(_s_), " of wonders!"));
            } vt_str_destroy(_s_);
            
            assert(vt_str_equals(vt_str_z(_s__), "hello world of "));
        } vt_str_destroy(_s__);

    } vt_str_destroy(sto);

    vt_str_t *s_strip = vt_str_create("   \nhello, world\t  ", alloctr);
    vt_str_strip(s_strip);
    assert(vt_str_len(s_strip) == 12);
    assert(vt_str_equals(vt_str_z(s_strip), "hello, world"));
    vt_str_destroy(s_strip);

    s_strip = vt_str_create("\n  ..,!  hello, world  ;;-!  \t\n", alloctr);
    vt_str_strip_punct(s_strip);
    assert(vt_str_len(s_strip) == 12);
    assert(vt_str_equals(vt_str_z(s_strip), "hello, world"));
    vt_str_destroy(s_strip);

    s_strip = vt_str_create("\n  ..,!  hello, world  ;;-!  \t\n", alloctr);
    vt_str_strip_c(s_strip, "\n .,;!\t-");
    assert(vt_str_len(s_strip) == 12);
    assert(vt_str_equals(vt_str_z(s_strip), "hello, world"));
    vt_str_destroy(s_strip);
    
    s_strip = vt_str_create("\n  ..,!  hello, world  ;;-!  \t\n", alloctr);
    vt_str_remove_c(s_strip, "\n .,;!\t-");
    assert(vt_str_len(s_strip) == 10);
    assert(vt_str_equals(vt_str_z(s_strip), "helloworld"));
    vt_str_destroy(s_strip);
    
    s_strip = vt_str_create("Here is a shopping list: apples and oranges, milk and sugar, and vinegar.", alloctr);
    vt_str_remove_all(s_strip, " and");
    assert(vt_str_equals(vt_str_z(s_strip), "Here is a shopping list: apples oranges, milk sugar, vinegar."));
    vt_str_destroy(s_strip);

    s_strip = vt_str_create("hello, world, world!", alloctr);
    vt_str_remove_last(s_strip, ", world");
    assert(vt_str_equals(vt_str_z(s_strip), "hello, world!"));
    assert(vt_str_len(s_strip) == 13);
    vt_str_destroy(s_strip);

    s_strip = vt_str_create("A A A", alloctr);
    vt_str_apply(s_strip, apply_func);
    assert(vt_str_equals(vt_str_z(s_strip), "bbbbb"));
    vt_str_clear(s_strip);

    vt_str_appendf(s_strip, "%s %d %s", "I have", 2, "apples");
    printf("%s | len: %zu\n", vt_str_z(s_strip), vt_str_len(s_strip));
    assert(vt_str_equals(vt_str_z(s_strip), "I have 2 apples"));
    vt_str_clear(s_strip);

    vt_str_appendf(s_strip, "%s", "abc");
    printf("%s | len: %zu\n", vt_str_z(s_strip), vt_str_len(s_strip));
    vt_str_appendf(s_strip, "%s", "def");
    printf("%s | len: %zu\n", vt_str_z(s_strip), vt_str_len(s_strip));
    assert(vt_str_equals(vt_str_z(s_strip), "abcdef"));
    vt_str_destroy(s_strip);

    s_strip = vt_str_create_capacity(1, alloctr);
    vt_str_appendf(s_strip, "%s, %s. %s %d!", "hello", "world", "hi", 5);
    assert(vt_str_equals(vt_str_z(s_strip), "hello, world. hi 5!"));
    vt_str_destroy(s_strip);

    s_strip = vt_str_create("hello, world!", alloctr);
    vt_str_capitalize(s_strip);
    assert(vt_str_equals(vt_str_z(s_strip), "HELLO, WORLD!"));
    assert(!vt_str_is_numeric(vt_str_z(s_strip), 256));
    
    vt_str_set(s_strip, "123");
    assert(vt_str_is_numeric(vt_str_z(s_strip), 256));

    vt_str_set(s_strip, "564.025");
    assert(vt_str_is_numeric(vt_str_z(s_strip), 256));
    vt_str_destroy(s_strip);

    vt_str_t *sbetween = vt_str_split_between(NULL, "https :1.8.8.8@ world", ":", "@");
    assert(vt_str_equals(vt_str_z(sbetween), "1.8.8.8"));
    vt_str_destroy(sbetween);

    vt_str_t *s_val_test = vt_str_create("123456789", alloctr); {
        assert(vt_str_len(s_val_test) == 9);
        assert(vt_str_capacity(s_val_test) == 9);

        ((char*)s_val_test->ptr)[4] = '\0';
        assert(vt_str_validate_len(s_val_test) == 4);

        assert(vt_str_len(s_val_test) == 4);
        assert(vt_str_capacity(s_val_test) == 9);

    } vt_str_destroy(s_val_test);

    vt_mallocator_destroy(alloctr);
    return 0;
}

void print_str(void *ptr, size_t i) {
    vt_str_destroy((vt_str_t*)ptr);
}

void apply_func(char *c, size_t i) {
    *c = 'b';
}
