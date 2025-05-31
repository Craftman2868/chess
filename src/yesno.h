#ifndef YESNO_H
#define YESNO_H

enum {
    CANCEL = -1,
    YES = 0,
    NO = 1,
} typedef yesno_t;

void open_yesno(char *title, void (*callback)(yesno_t));

#endif /* YESNO_H */
