#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

// --- TIỆN ÍCH HỖ TRỢ THỜI GIAN ---
string timeToString(time_t t)
{
    char buffer[80];
    struct tm *timeinfo = localtime(&t);
    strftime(buffer, 80, "%d/%m/%Y", timeinfo);
    return string(buffer);
}

// --- CLASS: ROOM (PHÒNG) ---
class Room
{
private:
    string id;
    string type;
    double price;
    string status; // "Trong", "Co khach", "Don dep"

public:
    Room(string _id, string _type, double _price)
        : id(_id), type(_type), price(_price), status("Trong") {}

    string getId() const { return id; }
    string getStatus() const { return status; }
    double getPrice() const { return price; }

    void setStatus(string newStatus) { status = newStatus; }
    void setPrice(double newPrice) { price = newPrice; }

    void display() const
    {
        cout << left << setw(10) << id
             << setw(15) << type
             << setw(15) << (long long)price
             << setw(15) << status << endl;
    }
};

// --- CLASS: CUSTOMER (KHÁCH HÀNG) ---
class Customer
{
private:
    string id; // CCCD
    string name;
    string phone;
    int birthYear;

public:
    Customer(string _id, string _name, string _phone, int _year)
        : id(_id), name(_name), phone(_phone), birthYear(_year) {}

    string getId() const { return id; }
    string getName() const { return name; }

    void updateInfo(string _name, string _phone, int _year)
    {
        name = _name;
        phone = _phone;
        birthYear = _year;
    }

    void display() const
    {
        cout << left << setw(15) << id
             << setw(20) << name
             << setw(15) << phone
             << setw(10) << birthYear << endl;
    }
};

// --- STRUCT: BOOKING (ĐANG LƯU TRÚ) ---
struct Booking
{
    string bookingID;
    string customerID;
    string roomID;
    time_t checkInTime;
};

// --- STRUCT: TRANSACTION (LỊCH SỬ) ---
struct Transaction
{
    string customerID;
    string roomID;
    time_t checkInTime;
    time_t checkOutTime;
    double totalAmount;
};

// --- CLASS: HOTEL MANAGER (QUẢN LÝ CHÍNH) ---
class HotelManager
{
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Booking> activeBookings;
    vector<Transaction> history;
    const string ADMIN_USER = "admin";
    const string ADMIN_PASS = "12345";

public:
    // 1. HỆ THỐNG ĐĂNG NHẬP
    bool login()
    {
        string u, p;
        cout << "\n=== DANG NHAP HE THONG LE TAN ===\n";
        cout << "Username: ";
        cin >> u;
        cout << "Password: ";
        cin >> p;
        if (u == ADMIN_USER && p == ADMIN_PASS)
            return true;
        return false;
    }

    // 2. QUẢN LÝ PHÒNG
    void addRoom()
    {
        string id, type;
        double price;
        cout << "Nhap Ma Phong: ";
        cin >> id;
        // Kiểm tra trùng
        for (auto &r : rooms)
            if (r.getId() == id)
            {
                cout << "Ma phong da ton tai!\n";
                return;
            }

        cout << "Nhap Loai Phong (Don/Doi/VIP): ";
        cin.ignore();
        getline(cin, type);
        cout << "Nhap Gia Phong/Dem: ";
        cin >> price;
        rooms.push_back(Room(id, type, price));
        cout << "Them phong thanh cong!\n";
    }

    void listRooms()
    {
        cout << "\n--- DANH SACH PHONG ---\n";
        cout << left << setw(10) << "Ma" << setw(15) << "Loai" << setw(15) << "Gia" << setw(15) << "Trang thai" << endl;
        for (const auto &r : rooms)
            r.display();
    }

    void updateRoomStatus()
    {
        string id, newStatus;
        cout << "\n--- CAP NHAT TRANG THAI PHONG ---\n";
        cout << "Nhap Ma Phong: ";
        cin >> id;

        // Tìm phòng
        bool found = false;
        for (auto &r : rooms)
        {
            if (r.getId() == id)
            {
                found = true;
                cout << "\nCac trang thai co san:\n";
                cout << "  1. Trong\n";
                cout << "  2. Co khach\n";
                cout << "  3. Don dep\n";
                cout << "Nhap trang thai moi (Trong/Co khach/Don dep): ";
                cin.ignore();
                getline(cin, newStatus);

                // Kiểm tra trang thái hợp lệ
                if (newStatus == "Trong" || newStatus == "Co khach" || newStatus == "Don dep")
                {
                    r.setStatus(newStatus);
                    cout << "Cap nhat trang thai phong thanh cong!\n";
                    cout << "\nThong tin phong sau khi cap nhat:\n";
                    cout << left << setw(10) << "Ma" << setw(15) << "Loai" << setw(15) << "Gia" << setw(15) << "Trang thai" << endl;
                    r.display();
                }
                else
                {
                    cout << "Trang thai khong hop le! Vui long chon lai.\n";
                }
                break;
            }
        }

        if (!found)
        {
            cout << "Khong tim thay phong voi ma: " << id << "\n";
        }
    }

    // 3. QUẢN LÝ KHÁCH HÀNG
    void addCustomer()
    {
        string id, name, phone;
        int year;
        cout << "Nhap CCCD: ";
        cin >> id;
        for (auto &c : customers)
            if (c.getId() == id)
            {
                cout << "Khach hang da ton tai!\n";
                return;
            }

        cout << "Ho Ten: ";
        cin.ignore();
        getline(cin, name);
        cout << "SDT: ";
        cin >> phone;
        cout << "Nam sinh: ";
        cin >> year;
        customers.push_back(Customer(id, name, phone, year));
        cout << "Them khach hang thanh cong!\n";
    }

    void listCustomers()
    {
        cout << "\n--- DANH SACH KHACH HANG ---\n";
        cout << left << setw(15) << "CCCD" << setw(20) << "Ten" << setw(15) << "SDT" << setw(10) << "Nam Sinh" << endl;
        for (const auto &c : customers)
            c.display();
    }

    void updateCustomer()
    {
        string id;
        cout << "\n--- CAP NHAT THONG TIN KHACH HANG ---\n";
        cout << "Nhap CCCD khach hang can cap nhat: ";
        cin >> id;

        // Tìm khách hàng
        bool found = false;
        for (auto &c : customers)
        {
            if (c.getId() == id)
            {
                found = true;
                string newName, newPhone;
                int newYear;

                cout << "Ho ten moi: ";
                cin.ignore();
                getline(cin, newName);
                cout << "SDT moi: ";
                cin >> newPhone;
                cout << "Nam sinh moi: ";
                cin >> newYear;

                c.updateInfo(newName, newPhone, newYear);
                cout << "Cap nhat thong tin khach hang thanh cong!\n";
                cout << "\nThong tin sau khi cap nhat:\n";
                cout << left << setw(15) << "CCCD" << setw(20) << "Ten" << setw(15) << "SDT" << setw(10) << "Nam Sinh" << endl;
                c.display();
                break;
            }
        }

        if (!found)
        {
            cout << "Khong tim thay khach hang voi CCCD: " << id << "\n";
        }
    }

    // 4. CHECK-IN
    void checkIn()
    {
        string rID, cID;
        cout << "\n--- CHECK IN ---\n";
        cout << "Nhap Ma Phong muon dat: ";
        cin >> rID;

        // Kiểm tra phòng
        int rIndex = -1;
        for (size_t i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].getId() == rID)
            {
                rIndex = i;
                break;
            }
        }

        if (rIndex == -1)
        {
            cout << "Khong tim thay phong!\n";
            return;
        }
        if (rooms[rIndex].getStatus() != "Trong")
        {
            cout << "Phong khong san sang (Da co khach hoac dang don)!\n";
            return;
        }

        cout << "Nhap CCCD Khach hang: ";
        cin >> cID;
        bool cFound = false;
        for (const auto &c : customers)
            if (c.getId() == cID)
                cFound = true;
        if (!cFound)
        {
            cout << "Khach hang moi? Vui long tao thong tin khach hang truoc.\n";
            return;
        }

        // Nhập thời gian check-in từ nhân viên
        int day, month, year, hour, minute, second;
        cout << "\nNhap thoi gian CHECK-IN:\n";
        cout << "Ngay (1-31): ";
        cin >> day;
        cout << "Thang (1-12): ";
        cin >> month;
        cout << "Nam (e.g. 2025): ";
        cin >> year;

        struct tm timeInfo = {};
        timeInfo.tm_mday = day;
        timeInfo.tm_mon = month - 1;
        timeInfo.tm_year = year - 1900;
        time_t checkInTime = mktime(&timeInfo);

        // Thực hiện Check-in
        Booking b;
        b.bookingID = "BK" + to_string(time(0)); // Mã booking ngẫu nhiên theo time
        b.roomID = rID;
        b.customerID = cID;
        b.checkInTime = checkInTime; // Sử dụng thời gian nhập vào

        activeBookings.push_back(b);
        rooms[rIndex].setStatus("Co khach");
        cout << "Check-in thanh cong! Ma Booking: " << b.bookingID << endl;
        cout << "Thoi gian check-in: " << timeToString(checkInTime) << endl;
    }

    // 5. CHECK-OUT
    void checkOut()
    {
        string rID;
        cout << "\n--- CHECK OUT ---\n";
        cout << "Nhap Ma Phong can tra: ";
        cin >> rID;

        int bIndex = -1;
        for (size_t i = 0; i < activeBookings.size(); i++)
        {
            if (activeBookings[i].roomID == rID)
            {
                bIndex = i;
                break;
            }
        }

        if (bIndex == -1)
        {
            cout << "Phong nay hien khong co booking nao!\n";
            return;
        }

        // Nhập thời gian check-out từ nhân viên
        int day, month, year, hour, minute, second;
        cout << "\nNhap thoi gian CHECK-OUT:\n";
        cout << "Ngay (1-31): ";
        cin >> day;
        cout << "Thang (1-12): ";
        cin >> month;
        cout << "Nam (e.g. 2025): ";
        cin >> year;

        struct tm timeInfo = {};
        timeInfo.tm_mday = day;
        timeInfo.tm_mon = month - 1;
        timeInfo.tm_year = year - 1900;
        time_t checkOutTime = mktime(&timeInfo);

        // Tính toán tiền
        Booking &b = activeBookings[bIndex];
        double seconds = difftime(checkOutTime, b.checkInTime);
        double days = ceil(seconds / (24 * 3600));
        if (days < 1)
            days = 1; // Tối thiểu tính 1 ngày

        // Lấy giá phòng
        double price = 0;
        for (auto &r : rooms)
        {
            if (r.getId() == b.roomID)
            {
                price = r.getPrice();
                r.setStatus("Don dep"); // Cập nhật trạng thái sau khi khách đi
                break;
            }
        }

        double total = days * price;

        // Lưu lịch sử
        Transaction trans;
        trans.customerID = b.customerID;
        trans.roomID = b.roomID;
        trans.checkInTime = b.checkInTime;
        trans.checkOutTime = checkOutTime;
        trans.totalAmount = total;
        history.push_back(trans);

        // Xoá booking đang hoạt động
        activeBookings.erase(activeBookings.begin() + bIndex);

        cout << "Check-out thanh cong!\n";
        cout << "Thoi gian o: " << days << " dem.\n";
        cout << "TONG TIEN THANH TOAN: " << (long long)total << " VND\n";
    }

    // 6. THỐNG KÊ LỊCH SỬ
    void viewHistory()
    {
        string cID;
        cout << "\n--- LICH SU GIAO DICH ---\n";
        cout << "Nhap CCCD khach hang (hoac 'all' de xem tat ca): ";
        cin >> cID;

        cout << left << setw(15) << "Khach" << setw(10) << "Phong"
             << setw(25) << "Check-in" << setw(25) << "Check-out" << "Thanh Tien" << endl;

        double sumRevenue = 0;
        for (const auto &t : history)
        {
            if (cID == "all" || t.customerID == cID)
            {
                cout << left << setw(15) << t.customerID
                     << setw(10) << t.roomID
                     << setw(25) << timeToString(t.checkInTime)
                     << setw(25) << timeToString(t.checkOutTime)
                     << (long long)t.totalAmount << endl;
                sumRevenue += t.totalAmount;
            }
        }
        if (cID == "all")
            cout << "-> TONG DOANH THU: " << (long long)sumRevenue << endl;
    }

    void initData()
    {
        rooms.push_back(Room("P101", "Don", 300000));
        rooms.push_back(Room("P102", "Doi", 500000));
        rooms.push_back(Room("P201", "VIP", 1000000));
        customers.push_back(Customer("001", "Nguyen Van A", "090123456", 1990));
    }
};

// --- MAIN PROGRAM ---
int main()
{
    HotelManager hotel;
    hotel.initData(); // Nạp dữ liệu mẫu

    if (!hotel.login())
    {
        cout << "Dang nhap that bai!\n";
        return 0;
    }

    int choice;
    do
    {
        cout << "\n=== QUAN LY KHACH SAN ===\n";
        cout << "1. Quan ly Phong (Xem/Them)\n";
        cout << "2. Quan ly Khach Hang (Xem/Them)\n";
        cout << "3. Dat Phong (Check-in)\n";
        cout << "4. Tra Phong (Check-out)\n";
        cout << "5. Thong ke lich su\n";
        cout << "0. Thoat\n";
        cout << "Chon chuc nang: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            int pChoice;
            cout << "  1. Xem danh sach\n  2. Them phong\n  3. Cap nhat trang thai\n  Chon: ";
            cin >> pChoice;
            if (pChoice == 1)
                hotel.listRooms();
            else if (pChoice == 2)
                hotel.addRoom();
            else if (pChoice == 3)
                hotel.updateRoomStatus();
            break;
        case 2:
            int cChoice;
            cout << "  1. Xem danh sach\n  2. Them khach\n  3. Cap nhat thong tin khach\n  Chon: ";
            cin >> cChoice;
            if (cChoice == 1)
                hotel.listCustomers();
            else if (cChoice == 2)
                hotel.addCustomer();
            else if (cChoice == 3)
                hotel.updateCustomer();
            break;
        case 3:
            hotel.checkIn();
            break;
        case 4:
            hotel.checkOut();
            break;
        case 5:
            hotel.viewHistory();
            break;
        case 0:
            cout << "Tam biet!";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}