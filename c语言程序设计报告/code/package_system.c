#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PACKAGES 100
#define MAX_TRACKING 20
#define MAX_NAME 20
#define MAX_PHONE 15
#define MAX_COMPANY 20
#define MAX_SHELF 10
#define MAX_DATE 12
#define MAX_STATUS 8
#define FILENAME "packages.txt"
/* 快递数据结构 */
typedef struct {
char tracking_no[MAX_TRACKING];   /* 快递单号 */
char receiver[MAX_NAME];          /* 收件人姓名 */
char phone[MAX_PHONE];            /* 收件人电话 */
char company[MAX_COMPANY];        /* 快递公司 */
char shelf[MAX_SHELF];            /* 货架编号 */
char date[MAX_DATE];              /* 入库日期 */
char status[MAX_STATUS];          /* 状态: 待取/已取/已退 */
} Package;
/* 全局数据 */
Package packages[MAX_PACKAGES];
int package_count = 0;
/* ---------- 文件读写 ---------- */
/* 从文件加载数据 */
void load_data() {
FILE *fp = fopen(FILENAME, "r");
if (fp == NULL) {
return; /* 首次运行无文件，正常 */
}
package_count = 0;
while (package_count < MAX_PACKAGES &&
fscanf(fp, "%19s %19s %14s %19s %9s %11s %7s",
packages[package_count].tracking_no,
packages[package_count].receiver,
packages[package_count].phone,
packages[package_count].company,
packages[package_count].shelf,
packages[package_count].date,
packages[package_count].status) == 7) {
package_count++;
}
fclose(fp);
}
/* 保存数据到文件 */
void save_data() {
FILE *fp = fopen(FILENAME, "w");
if (fp == NULL) {
printf("错误：无法写入文件 %s\n", FILENAME);
return;
}
int i;
for (i = 0; i < package_count; i++) {
fprintf(fp, "%s %s %s %s %s %s %s\n",
packages[i].tracking_no,
packages[i].receiver,
packages[i].phone,
packages[i].company,
packages[i].shelf,
packages[i].date,
packages[i].status);
}
fclose(fp);
}
/* ---------- 工具函数 ---------- */
/* 根据快递单号查找，返回索引，找不到返回-1 */
int find_by_tracking(const char *tracking_no) {
int i;
for (i = 0; i < package_count; i++) {
if (strcmp(packages[i].tracking_no, tracking_no) == 0) {
return i;
}
}
return -1;
}
/* 安全读取字符串（去除末尾换行） */
void read_string(char *buf, int size) {
fgets(buf, size, stdin);
int len = (int)strlen(buf);
if (len > 0 && buf[len - 1] == '\n') {
buf[len - 1] = '\0';
}
}
/* ---------- 1. 快递录入 ---------- */
void add_package() {
if (package_count >= MAX_PACKAGES) {
printf("错误：快递数量已达上限(%d)，无法继续录入。\n", MAX_PACKAGES);
return;
}
Package p;
printf("\n========== 快递录入 ==========\n");
printf("请输入快递单号: ");
read_string(p.tracking_no, MAX_TRACKING);
/* 检查单号唯一性 */
if (find_by_tracking(p.tracking_no) != -1) {
printf("错误：快递单号 [%s] 已存在，请勿重复录入。\n", p.tracking_no);
return;
}
printf("请输入收件人姓名: ");
read_string(p.receiver, MAX_NAME);
printf("请输入收件人电话: ");
read_string(p.phone, MAX_PHONE);
printf("请输入快递公司: ");
read_string(p.company, MAX_COMPANY);
printf("请输入货架编号: ");
read_string(p.shelf, MAX_SHELF);
printf("请输入入库日期(如2025-06-18): ");
read_string(p.date, MAX_DATE);
strcpy(p.status, "待取");
packages[package_count] = p;
package_count++;
save_data();
printf("录入成功！当前共有 %d 条快递记录。\n", package_count);
}
/* ---------- 2. 快递查询 ---------- */
void query_package() {
if (package_count == 0) {
printf("暂无快递记录。\n");
return;
}
int choice;
printf("\n========== 快递查询 ==========\n");
printf("1. 按快递单号查询\n");
printf("2. 按收件人姓名查询\n");
printf("请选择查询方式: ");
scanf("%d", &choice);
getchar(); /* 吃掉回车 */
if (choice == 1) {
char tracking_no[MAX_TRACKING];
printf("请输入快递单号: ");
read_string(tracking_no, MAX_TRACKING);
int idx = find_by_tracking(tracking_no);
if (idx == -1) {
printf("未找到该快递单号 [%s]。\n", tracking_no);
} else {
Package *s = &packages[idx];
printf("\n快递单号: %s\n收件人: %s\n电话: %s\n快递公司: %s\n货架编号: %s\n入库日期: %s\n状态: %s\n",
s->tracking_no, s->receiver, s->phone, s->company, s->shelf, s->date, s->status);
}
} else if (choice == 2) {
char name[MAX_NAME];
printf("请输入收件人姓名: ");
read_string(name, MAX_NAME);
int found = 0, i;
printf("\n查询结果:\n");
printf("----------------------------------------------------------------------\n");
printf("%-18s %-10s %-20s %-10s %-12s %-6s\n",
"快递单号", "收件人", "快递公司", "货架", "入库日期", "状态");
printf("----------------------------------------------------------------------\n");
for (i = 0; i < package_count; i++) {
if (strstr(packages[i].receiver, name) != NULL) {
printf("%-18s %-10s %-20s %-10s %-12s %-6s\n",
packages[i].tracking_no, packages[i].receiver,
packages[i].company, packages[i].shelf,
packages[i].date, packages[i].status);
found++;
}
}
printf("----------------------------------------------------------------------\n");
printf("共找到 %d 条记录。\n", found);
} else {
printf("无效选择。\n");
}
}
/* ---------- 3. 取件管理 ---------- */
void pickup_package() {
if (package_count == 0) {
printf("暂无快递记录。\n");
return;
}
char tracking_no[MAX_TRACKING];
printf("\n========== 取件管理 ==========\n");
printf("请输入要取件的快递单号: ");
read_string(tracking_no, MAX_TRACKING);
int idx = find_by_tracking(tracking_no);
if (idx == -1) {
printf("未找到该快递单号 [%s]。\n", tracking_no);
return;
}
if (strcmp(packages[idx].status, "已取") == 0) {
printf("该快递已被取走，无需重复操作。\n");
return;
}
if (strcmp(packages[idx].status, "已退") == 0) {
printf("该快递已被退回，无法取件。\n");
return;
}
strcpy(packages[idx].status, "已取");
save_data();
printf("取件成功！快递 [%s] 已标记为已取。\n", tracking_no);
}
/* ---------- 4. 退件处理 ---------- */
void return_package() {
if (package_count == 0) {
printf("暂无快递记录。\n");
return;
}
char tracking_no[MAX_TRACKING];
printf("\n========== 退件处理 ==========\n");
printf("请输入要退回的快递单号: ");
read_string(tracking_no, MAX_TRACKING);
int idx = find_by_tracking(tracking_no);
if (idx == -1) {
printf("未找到该快递单号 [%s]。\n", tracking_no);
return;
}
if (strcmp(packages[idx].status, "已退") == 0) {
printf("该快递已被退回，无需重复操作。\n");
return;
}
strcpy(packages[idx].status, "已退");
save_data();
printf("退件成功！快递 [%s] 已标记为已退。\n", tracking_no);
}
/* ---------- 5. 信息修改 ---------- */
void modify_package() {
if (package_count == 0) {
printf("暂无快递记录。\n");
return;
}
char tracking_no[MAX_TRACKING];
printf("\n========== 信息修改 ==========\n");
printf("请输入要修改的快递单号: ");
read_string(tracking_no, MAX_TRACKING);
int idx = find_by_tracking(tracking_no);
if (idx == -1) {
printf("未找到该快递单号 [%s]。\n", tracking_no);
return;
}
Package *s = &packages[idx];
printf("(直接回车保留原值)\n");
char buf[MAX_NAME];
printf("收件人姓名 (当前: %s): ", s->receiver);
read_string(buf, MAX_NAME);
if (strlen(buf) > 0) strcpy(s->receiver, buf);
printf("收件人电话 (当前: %s): ", s->phone);
read_string(buf, MAX_PHONE);
if (strlen(buf) > 0) strcpy(s->phone, buf);
printf("快递公司 (当前: %s): ", s->company);
read_string(buf, MAX_COMPANY);
if (strlen(buf) > 0) strcpy(s->company, buf);
printf("货架编号 (当前: %s): ", s->shelf);
read_string(buf, MAX_SHELF);
if (strlen(buf) > 0) strcpy(s->shelf, buf);
printf("入库日期 (当前: %s): ", s->date);
read_string(buf, MAX_DATE);
if (strlen(buf) > 0) strcpy(s->date, buf);
save_data();
printf("修改成功！\n");
}
/* ---------- 6. 数据统计 ---------- */
void show_statistics() {
if (package_count == 0) {
printf("暂无快递记录，无法统计。\n");
return;
}
int pending = 0, picked = 0, returned = 0;
int i;
for (i = 0; i < package_count; i++) {
if (strcmp(packages[i].status, "待取") == 0) pending++;
else if (strcmp(packages[i].status, "已取") == 0) picked++;
else if (strcmp(packages[i].status, "已退") == 0) returned++;
}
printf("\n========== 数据统计 ==========\n");
printf("快递总数: %d\n", package_count);
printf("待取件数: %d (占 %.1f%%)\n", pending, 100.0 * pending / package_count);
printf("已取件数: %d (占 %.1f%%)\n", picked, 100.0 * picked / package_count);
printf("已退件数: %d (占 %.1f%%)\n", returned, 100.0 * returned / package_count);
/* 按快递公司统计 */
printf("\n--- 各快递公司快递数量 ---\n");
/* 简单的去重统计 */
char companies[MAX_PACKAGES][MAX_COMPANY];
int counts[MAX_PACKAGES] = {0};
int comp_count = 0;
for (i = 0; i < package_count; i++) {
int found = 0, j;
for (j = 0; j < comp_count; j++) {
if (strcmp(companies[j], packages[i].company) == 0) {
counts[j]++;
found = 1;
break;
}
}
if (!found) {
strcpy(companies[comp_count], packages[i].company);
counts[comp_count] = 1;
comp_count++;
}
}
for (i = 0; i < comp_count; i++) {
printf("  %s: %d 件\n", companies[i], counts[i]);
}
/* 货架分布 */
printf("\n--- 各货架快递分布 ---\n");
char shelves[MAX_PACKAGES][MAX_SHELF];
int shelf_counts[MAX_PACKAGES] = {0};
int shelf_count = 0;
for (i = 0; i < package_count; i++) {
int found = 0, j;
for (j = 0; j < shelf_count; j++) {
if (strcmp(shelves[j], packages[i].shelf) == 0) {
shelf_counts[j]++;
found = 1;
break;
}
}
if (!found) {
strcpy(shelves[shelf_count], packages[i].shelf);
shelf_counts[shelf_count] = 1;
shelf_count++;
}
}
for (i = 0; i < shelf_count; i++) {
printf("  货架 %s: %d 件\n", shelves[i], shelf_counts[i]);
}
}
/* ---------- 浏览所有快递 ---------- */
void browse_all() {
if (package_count == 0) {
printf("暂无快递记录。\n");
return;
}
printf("\n========== 全部快递信息 ==========\n");
printf("----------------------------------------------------------------------------------------\n");
printf("%-18s %-10s %-14s %-12s %-8s %-12s %-6s\n",
"快递单号", "收件人", "电话", "快递公司", "货架", "入库日期", "状态");
printf("----------------------------------------------------------------------------------------\n");
int i;
for (i = 0; i < package_count; i++) {
printf("%-18s %-10s %-14s %-12s %-8s %-12s %-6s\n",
packages[i].tracking_no, packages[i].receiver,
packages[i].phone, packages[i].company,
packages[i].shelf, packages[i].date,
packages[i].status);
}
printf("----------------------------------------------------------------------------------------\n");
printf("共 %d 条记录。\n", package_count);
}
/* ---------- 删除快递 ---------- */
void delete_package() {
if (package_count == 0) {
printf("暂无快递记录。\n");
return;
}
char tracking_no[MAX_TRACKING];
printf("\n========== 删除快递 ==========\n");
printf("请输入要删除的快递单号: ");
read_string(tracking_no, MAX_TRACKING);
int idx = find_by_tracking(tracking_no);
if (idx == -1) {
printf("未找到该快递单号 [%s]。\n", tracking_no);
return;
}
int i;
for (i = idx; i < package_count - 1; i++) {
packages[i] = packages[i + 1];
}
package_count--;
save_data();
printf("删除成功！快递 [%s] 已从系统中移除。\n", tracking_no);
}
/* ---------- 主函数 ---------- */
int main() {
load_data();
int choice;
do {
printf("\n");
printf("=================================================\n");
printf("          快递代收点管理系统\n");
printf("=================================================\n");
printf("  1. 快递录入          2. 快递查询\n");
printf("  3. 取件管理          4. 退件处理\n");
printf("  5. 信息修改          6. 数据统计\n");
printf("  7. 浏览全部          8. 删除快递\n");
printf("  0. 退出系统\n");
printf("=================================================\n");
printf("请输入您的选择: ");
if (scanf("%d", &choice) != 1) {
printf("输入无效，请输入数字。\n");
while (getchar() != '\n'); /* 清空输入缓冲区 */
choice = -1;
continue;
}
getchar(); /* 吃掉回车 */
switch (choice) {
case 1: add_package(); break;
case 2: query_package(); break;
case 3: pickup_package(); break;
case 4: return_package(); break;
case 5: modify_package(); break;
case 6: show_statistics(); break;
case 7: browse_all(); break;
case 8: delete_package(); break;
case 0:
save_data();
printf("数据已保存，感谢使用！\n");
break;
default:
printf("无效选项，请重新输入(0-8)。\n");
}
} while (choice != 0);
return 0;
}