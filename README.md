# Rendszerközeli Programozás Projekt Feladat

Rendszerközeli Programozás tantárgyhoz készülő projektfeladat


## Fordítás

A program nevének `chart`-nak kell lennie hogy futtatni lehessen

```bash
  gcc main.c -o chart
```
    
## Futtatás

Ha nincs megadva kapcsoló a program alapértelmezettként a `-send` és `-file` kapcsolókat használja. A futtatás szintaxisa `./chart [kapcsolók]...`

```bash
  ./chart
```

| **Kapcsolók** | **Leírás**         |
| :------------ | :----------------- |
| `--help`      | Súgó kiírása       |
| `--version`   | Verziószám kiírása |
| `--debug`     | Debug mód          |

| **Üzemmódok** | **Leírás**      |
| :------------ | :-------------- |
| `-send`       | Adatok küldése  |
| `-receive`    | Adatok fogadása |

| **Kommunikációs módok** | **Leírás**                             |
| :---------------------- | :------------------------------------- |
| `-file`                 | Fájl használata az adatok kűldéséhez   |
| `-receive`              | Socket használata az adatok kűldéséhez |
