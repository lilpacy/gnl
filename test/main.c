#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../get_next_line.h"

void test_file(const char *filename, const char *test_name) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("❌ %s: Failed to open file\n", test_name);
        return;
    }
    
    char *line;
    int line_count = 0;
    
    while ((line = get_next_line(fd)) != NULL) {
        printf("Line %d: %s", ++line_count, line);
        free(line);  // 確実にfree
        line = NULL; // NULLポインタにセット
    }
    
    close(fd);
    printf("✅ %s: Completed (%d lines)\n", test_name, line_count);
}

void test_with_valgrind(const char *filename, const char *test_name) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("❌ %s: Failed to open file\n", test_name);
        return;
    }
    
    char *line;
    int line_count = 0;
    
    while ((line = get_next_line(fd)) != NULL) {
        printf("Line %d: %s", ++line_count, line);
        free(line);  // 必ずfreeする
        line = NULL; // NULLポインタにセット
    }
    
    close(fd);
    printf("✅ %s: Completed (%d lines)\n", test_name, line_count);
}

void test_stdin(void) {
    printf("\n📝 Testing STDIN (type some lines, Ctrl+D to end):\n");
    char *line;
    int line_count = 0;
    
    while ((line = get_next_line(STDIN_FILENO)) != NULL) {
        printf("Line %d: %s", ++line_count, line);
        free(line);
    }
    printf("✅ STDIN test: Completed (%d lines)\n", line_count);
}

void test_multiple_fd(void) {
    printf("\n🔄 Testing multiple file descriptors:\n");
    int fd1 = open("files/normal.txt", O_RDONLY);
    int fd2 = open("files/single_char.txt", O_RDONLY);
    
    char *line1 = get_next_line(fd1);  // normal.txt から1行目
    printf("FD1 Line 1: %s", line1);
    free(line1);
    
    char *line2 = get_next_line(fd2);  // single_char.txt から1行目
    printf("FD2 Line 1: %s", line2);
    free(line2);
    
    line1 = get_next_line(fd1);  // normal.txt から2行目
    printf("FD1 Line 2: %s", line1);
    free(line1);
    
    close(fd1);
    close(fd2);
    printf("✅ Multiple FD test completed\n");
}


int main(void) {
    printf("🚀 Starting GNL Tests...\n\n");
    
    // 基本テスト
    test_file("files/empty.txt", "Empty file test");
    test_file("files/single_char.txt", "Single char test");
    test_file("files/normal.txt", "Normal file test");
    test_file("files/long_lines.txt", "Long lines test");
    
    // エッジケーステスト
    test_file("files/multiple_newlines.txt", "Multiple newlines test");
    test_file("files/no_newline.txt", "No newline test");
    test_file("files/with_null.txt", "Null character test");
    
    // エラーケーステスト
    test_file("/dev/null", "Dev null test");
    test_file("nonexistent", "Non-existent file test");
    test_file((char *)-1, "Invalid fd test");
    
    // 複数FDテスト
    test_multiple_fd();
    
    // STDINテスト（オプション）
    // test_stdin();  // 必要な場合にコメントアウトを解除
    
    printf("\n🏁 All tests completed!\n");
    return 0;
}