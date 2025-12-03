#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define NAME_LEN 50
#define FILENAME "inventori.txt"

#define COLOR_RESET   "\x1B[0m"
#define COLOR_RED     "\x1B[31m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN    "\x1B[36m"
#define COLOR_WHITE   "\x1B[37m"

struct Barang {
    int id;
    char nama[NAME_LEN];
    int stok;
    int harga;
};

struct Barang data[MAX];
int jumlahBarang = 0;

int my_strlen(const char *s);
void my_strcpy(char *dst, const char *src);
int my_strcmp(const char *a, const char *b);
char to_lower_char(char c);
char* my_stristr(const char *haystack, const char *needle);
int my_strncasecmp(const char *a, const char *b, int n);
void read_line(char *buf, int buflen);
int read_int(const char *prompt);
int cariRekursifById(int id, int idx);
void tampilBarang();
void tambahBarang();
void cariBarangById();
void cariBarangByName();
void updateBarang();
void hapusBarang();
void simpanKeFile();
void loadDariFile();

int main() {
    loadDariFile();
    printf(COLOR_GREEN "Inventori dimuat. Jumlah barang: %d\n" COLOR_RESET, jumlahBarang);

    while (1) {
        showMenu();
        int pil = read_int("Pilih menu: ");
        switch (pil) {
            case 1: tambahBarang(); break;
            case 2: tampilBarang(); break;
            case 3: cariBarangById(); break;
            case 4: cariBarangByName(); break;
            case 5: updateBarang(); break;
            case 6: hapusBarang(); break;
            case 7: simpanKeFile(); break;
            case 0:
                printf(COLOR_YELLOW "Simpan otomatis sebelum keluar? (1=Ya,0=Tidak): " COLOR_RESET);
                {
                    int s = read_int("");
                    if (s == 1) simpanKeFile();
                }
                printf(COLOR_MAGENTA "Keluar. Bye!\n" COLOR_RESET);
                return 0;
            default:
                printf(COLOR_RED "Pilihan tidak valid.\n" COLOR_RESET);
        }
    }
    return 0;
}

int my_strlen(const char *s) {
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}

void my_strcpy(char *dst, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

int my_strcmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return (int)(a[i]) - (int)(b[i]);
        i++;
    }
    return (int)(a[i]) - (int)(b[i]);
}

char to_lower_char(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

int my_strncasecmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; ++i) {
        char ca = to_lower_char(a[i]);
        char cb = to_lower_char(b[i]);
        if (ca != cb) return (int)ca - (int)cb;
        if (ca == '\0') return 0;
    }
    return 0;
}

char* my_stristr(const char *haystack, const char *needle) {
    int len_h = my_strlen(haystack);
    int len_n = my_strlen(needle);
    if (len_n == 0) return (char*)haystack;

    for (int i = 0; i <= len_h - len_n; ++i) {
        int match = 1;
        for (int j = 0; j < len_n; ++j) {
            char ch = to_lower_char(haystack[i+j]);
            char nd = to_lower_char(needle[j]);
            if (ch != nd) { match = 0; break; }
        }
        if (match) return (char*)(haystack + i);
    }
    return NULL;
}

void read_line(char *buf, int buflen) {
    if (fgets(buf, buflen, stdin) == NULL) {
        printf(COLOR_RED "\nInput berakhir. Keluar.\n" COLOR_RESET);
        exit(0);
    }
    int len = my_strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
}

int read_int(const char *prompt) {
    char line[128];
    int val;
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf(COLOR_RED "\nInput berakhir. Keluar.\n" COLOR_RESET);
            exit(0);
        }
        if (sscanf(line, "%d", &val) == 1) return val;
        printf(COLOR_RED "Input tidak valid. Coba lagi.\n" COLOR_RESET);
    }
}

int cariRekursifById(int id, int idx) {
    if (idx >= jumlahBarang) return -1;
    if (data[idx].id == id) return idx;
    return cariRekursifById(id, idx + 1);
}

void tambahBarang() {
    if (jumlahBarang >= MAX) {
        printf(COLOR_RED "Kapasitas penuh. Tidak dapat menambah barang.\n" COLOR_RESET);
        return;
    }

    struct Barang b;
    printf(COLOR_CYAN "\n=== Tambah Barang ===\n" COLOR_RESET);
    b.id = read_int("Masukkan ID (angka): ");

    printf("Masukkan nama (boleh spasi): ");
    read_line(b.nama, NAME_LEN);

    b.stok = read_int("Masukkan stok (angka): ");
    b.harga = read_int("Masukkan harga (angka): ");

    data[jumlahBarang] = b;
    jumlahBarang++;
    printf(COLOR_GREEN "Barang ditambahkan.\n" COLOR_RESET);
}

void tampilBarang() {
    printf(COLOR_CYAN "\n=== Daftar Barang (%d) ===\n" COLOR_RESET, jumlahBarang);
    if (jumlahBarang == 0) {
        printf(COLOR_YELLOW "Belum ada barang.\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < jumlahBarang; ++i) {
        struct Barang *p = &data[i];
        printf(COLOR_BLUE "%d) ID:%d | Nama:%s | Stok:%d | Harga:%d\n" COLOR_RESET,
            i+1, p->id, p->nama, p->stok, p->harga);
    }
}

void cariBarangById() {
    printf(COLOR_CYAN "\n=== Cari Barang (ID) ===\n" COLOR_RESET);
    int id = read_int("Masukkan ID: ");
    int idx = cariRekursifById(id, 0);
    if (idx == -1) {
        printf(COLOR_RED "Barang dengan ID %d tidak ditemukan.\n" COLOR_RESET, id);
    } else {
        struct Barang *p = &data[idx];
        printf(COLOR_YELLOW "Ditemukan: ID:%d | Nama:%s | Stok:%d | Harga:%d\n" COLOR_RESET,
                p->id, p->nama, p->stok, p->harga);
    }
}

void cariBarangByName() {
    printf(COLOR_CYAN "\n=== Cari Barang (Nama) ===\n" COLOR_RESET);
    char keyword[NAME_LEN];
    printf("Masukkan kata kunci (boleh sebagian): ");
    read_line(keyword, NAME_LEN);

    int found = 0;
    for (int i = 0; i < jumlahBarang; ++i) {
        if (my_stristr(data[i].nama, keyword) != NULL) {
            if (!found) printf(COLOR_YELLOW "Hasil pencarian:\n" COLOR_RESET);
            struct Barang *p = &data[i];
            printf(COLOR_WHITE "ID:%d | Nama:%s | Stok:%d | Harga:%d\n" COLOR_RESET,
                   p->id, p->nama, p->stok, p->harga);
            found = 1;
        }
    }
    if (!found) printf(COLOR_RED "Tidak ada barang yang cocok.\n" COLOR_RESET);
}

void updateBarang() {
    printf(COLOR_CYAN "\n=== Update Barang ===\n" COLOR_RESET);
    int id = read_int("Masukkan ID barang yang ingin diupdate: ");
    int idx = cariRekursifById(id, 0);
    if (idx == -1) {
        printf(COLOR_RED "Barang tidak ditemukan.\n" COLOR_RESET);
        return;
    }
    struct Barang *p = &data[idx];
    printf(COLOR_YELLOW "Data saat ini: ID:%d Nama:%s Stok:%d Harga:%d\n" COLOR_RESET,
           p->id, p->nama, p->stok, p->harga);

    char buffer[NAME_LEN];
    printf("Masukkan nama baru (kosong = tidak berubah): ");
    read_line(buffer, NAME_LEN);
    if (my_strlen(buffer) > 0) my_strcpy(p->nama, buffer);

    int newstok = read_int("Masukkan stok baru: ");
    p->stok = newstok;
    int newharga = read_int("Masukkan harga baru: ");
    p->harga = newharga;

    printf(COLOR_GREEN "Data berhasil diupdate.\n" COLOR_RESET);
}

void hapusBarang() {
    printf(COLOR_CYAN "\n=== Hapus Barang ===\n" COLOR_RESET);
    int id = read_int("Masukkan ID barang yang ingin dihapus: ");
    int idx = cariRekursifById(id, 0);
    if (idx == -1) {
        printf(COLOR_RED "Barang tidak ditemukan.\n" COLOR_RESET);
        return;
    }

    for (int i = idx; i < jumlahBarang - 1; ++i) {
        data[i].id = data[i+1].id;
        for (int k = 0; k < NAME_LEN; ++k) {
            data[i].nama[k] = data[i+1].nama[k];
            if (data[i+1].nama[k] == '\0') break;
        }
        data[i].stok = data[i+1].stok;
        data[i].harga = data[i+1].harga;
    }
    jumlahBarang--;
    printf(COLOR_GREEN "Barang berhasil dihapus.\n" COLOR_RESET);
}

void simpanKeFile() {
    FILE *f = fopen(FILENAME, "w");
    if (!f) {
        printf(COLOR_RED "Gagal membuka file untuk menyimpan.\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < jumlahBarang; ++i) {
        fprintf(f, "%d|%s|%d|%d\n",
                data[i].id, data[i].nama, data[i].stok, data[i].harga);
    }
    fclose(f);
    printf(COLOR_GREEN "Data disimpan ke file '%s'.\n" COLOR_RESET, FILENAME);
}

void loadDariFile() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f) != NULL) {
        int len = my_strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';

        char *p = line;
        char token[4][NAME_LEN];
        int t = 0, pos = 0;
        for (int i = 0; ; ++i) {
            char c = line[i];
            if (c == '|' || c == '\0') {
                token[t][pos] = '\0';
                t++;
                pos = 0;
                if (c == '\0' || t >= 4) break;
            } else {
                if (pos < NAME_LEN-1) token[t][pos++] = c;
            }
        }
        if (t >= 4) {
            if (jumlahBarang < MAX) {
                data[jumlahBarang].id = atoi(token[0]);
                my_strcpy(data[jumlahBarang].nama, token[1]);
                data[jumlahBarang].stok = atoi(token[2]);
                data[jumlahBarang].harga = atoi(token[3]);
                jumlahBarang++;
            }
        }
    }
    fclose(f);
}

void showMenu() {
    printf(COLOR_CYAN "\n=== MENU INVENTORI ===\n" COLOR_RESET);
    printf(COLOR_GREEN "1. Tambah Barang\n" COLOR_RESET);
    printf(COLOR_GREEN "2. Lihat Semua Barang\n" COLOR_RESET);
    printf(COLOR_GREEN "3. Cari Barang (ID)\n" COLOR_RESET);
    printf(COLOR_GREEN "4. Cari Barang (Nama)\n" COLOR_RESET);
    printf(COLOR_GREEN "5. Update Barang\n" COLOR_RESET);
    printf(COLOR_GREEN "6. Hapus Barang\n" COLOR_RESET);
    printf(COLOR_GREEN "7. Simpan ke File\n" COLOR_RESET);
    printf(COLOR_YELLOW "0. Keluar\n" COLOR_RESET);
}
