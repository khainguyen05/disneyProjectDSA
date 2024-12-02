#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
using namespace std;

// Struct ??i di?n cho d? li?u Disney
struct Disney {
    string Date;
    float Open_Price;
    float High_Price;
    float Low_Price;
    float Close_Price;
    int Volume;
};

// Struct ??i di?n cho node trong cây nh? phân
struct TreeNode {
    Disney data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Disney x) : data(x), left(nullptr), right(nullptr) {}
};

// Hàm t?o ??i t??ng Disney t? m?t chu?i d? li?u
Disney CreateObject(const string& line) {
    Disney d;
    try {
        stringstream ss(line);
        string temp;

        getline(ss, d.Date, ',');
        getline(ss, temp, ','); d.Open_Price = stof(temp);
        getline(ss, temp, ','); d.High_Price = stof(temp);
        getline(ss, temp, ','); d.Low_Price = stof(temp);
        getline(ss, temp, ','); d.Close_Price = stof(temp);
        getline(ss, temp, ','); d.Volume = stoi(temp);
    }
    catch (const exception& e) {
        cout << "Error parsing line: " << line << ". Exception: " << e.what() << endl;
    }
    return d;
}

// Hàm chèn d? li?u vào cây nh? phân tìm ki?m theo ngày (Date)
TreeNode* insert(TreeNode* root, Disney x) {
    if (root == nullptr) {
        return new TreeNode(x);
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (x.Date < current->data.Date) {
            current = current->left;
        }
        else if (x.Date > current->data.Date) {
            current = current->right;
        }
        else {
            return root; // Ngày ?ã t?n t?i
        }
    }

    if (x.Date < parent->data.Date) {
        parent->left = new TreeNode(x);
    }
    else {
        parent->right = new TreeNode(x);
    }
    return root;
}

//Ham doc du lieu tu file
void readFromFile(TreeNode*& root, const string& filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        cout << "Could not open file " << filename << endl;
        return;
    }

    string line;
    bool isFirstLine = true; // To skip the header row
    while (getline(input, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (!line.empty()) {
            try {
                Disney data = CreateObject(line);
                root = insert(root, data);
            }
            catch (const exception& e) {
                cout << "Skipping invalid line: " << line << endl;
            }
        }
    }
    input.close();
    cout << "File read successfully." << endl;
}


void inorderTraversal(TreeNode* root, int& count) {
    if (root == nullptr || count <= 0) return;

    stack<TreeNode*> s;
    TreeNode* current = root;

    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        if (count > 0) {
            cout << "Date: " << current->data.Date
                << ", Open Price: " << current->data.Open_Price
                << ", High Price: " << current->data.High_Price
                << ", Low Price: " << current->data.Low_Price
                << ", Close Price: " << current->data.Close_Price
                << ", Volume: " << current->data.Volume << endl;
            count--;
        }

        current = current->right;
    }
}
//Tim ngay co gia tri lon nhat theo tieu chi
Disney findMax(TreeNode* root, const string& type) {
    if (root == nullptr) {
        throw runtime_error("Tree is empty!");
    }

    stack<TreeNode*> s;
    TreeNode* current = root;
    Disney maxDisney = root->data;

    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        if ((type == "Open_Price" && current->data.Open_Price > maxDisney.Open_Price) ||
            (type == "High_Price" && current->data.High_Price > maxDisney.High_Price) ||
            (type == "Low_Price" && current->data.Low_Price > maxDisney.Low_Price) ||
            (type == "Close_Price" && current->data.Close_Price > maxDisney.Close_Price) ||
            (type == "Volume" && current->data.Volume > maxDisney.Volume)) {
            maxDisney = current->data;
        }

        current = current->right;
    }
    return maxDisney;
}
TreeNode* minNode(TreeNode* root)
{
    if (root == NULL)
    {
        return NULL;
    }
    TreeNode* current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}
// Ham tach ngay,thangm, nam tu choi yyyy-mm-dd
void splitDate(const std::string& dateStr, int& year, int& month, int& day)
{
    sscanf_s(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
}
//Xoa mot Node theo ngay
TreeNode* deleteNodeByDate(TreeNode* root, int year, int month, int day)
{
    if (root == NULL)
    {
        return root;
    }
    //tac nay thang nang tu node root->data.Date
    int nodeYear, nodeMonth, nodeDay;
    splitDate(root->data.Date, nodeYear, nodeMonth, nodeDay);
    if (year < nodeYear || (year == nodeYear && month < nodeMonth) || (year == nodeYear && month == nodeMonth && day < nodeDay))
    {
        //duyet ben trai
        root->left =deleteNodeByDate(root->left, year,month,day);
    }
    else if (year > nodeYear || (year == nodeYear && month > nodeMonth) || (year == nodeYear && month == nodeMonth && day > nodeDay))
    {
        //duyet ben phai
        root->right = deleteNodeByDate(root->right, year, month, day);
    }
    else {
        //Neu node can xoa 
        if (root->left == NULL and root->right == NULL)//Khong co con
        {
            delete root;
            return NULL;
        }
        else if (root->left == NULL) //cay co con phai
        {
            TreeNode* tmp = root->right; //luu con phai vao node tam
            delete root; 
            return tmp; 
        }
        else if (root->right == NULL)// chi co con trai
        {
            TreeNode* tmp = root->left; //luu con trai vao node tam
            delete root;
            return tmp;
        }
        else// neu cay co ca con trai va con phai

        {
            TreeNode* temp = minNode(root->right); //tim node nho nhat ben phai de luu vao node tam
            int tempYear, tempMonth, tempDay;
            splitDate(temp->data.Date, tempYear, tempMonth, tempDay);
            root->data = temp->data;//gan du lieu cua node nho nhat vao node can xoa
            root->right = deleteNodeByDate(root->right, tempYear, tempMonth, tempDay);// xoa node nho nhat o cay con phai
        }
    }
    return root;
}
void writeFile(TreeNode* root, const string& filename)
{
    ofstream output(filename);
    if (!output.is_open())
    {
        cout << "Khong the mo file: " << filename << endl;
        return;
    }
    stack<TreeNode*> s;
    TreeNode* current = root;
    //lap lai cho den khi tat ca node duoc duyet
    while (current != nullptr || !s.empty()) {
        //duyet trai va push vao stack
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }
        //lay node tu stack ra de xu ly
        current = s.top();
        //xoa node khoi stack
        s.pop();
        //Ghi du lieu cua node hien tai vao file
        output << current->data.Date << ","
            << current->data.Open_Price << ","
            << current->data.High_Price << ","
            << current->data.Low_Price << ","
            << current->data.Close_Price << ","
            << current->data.Volume << "\n";
        //duyet phai
        current = current->right;
    }

    output.close();
    cout << "Du lieu da duoc ghi vao file: " << filename << endl;
}
void displayMenu() {
    cout << "\n====== MENU ======\n";
    cout << "1. Xuat thong tin theo thu tu tang dan (Inorder Traversal)\n";
    cout << "2. Tim ngay cao nhat theo tieu chi (Open_price,Closed_price, Volume)\n";
    cout << "3. Chinh sua cay: Them thong tin vao cay\n";  
    cout << "4. Chinh sua cay: Xoa du lieu theo (yyyy-mm-dd)\n";
    cout << "5. Thoat\n";
    cout << "==================\n";
    cout << "Nhap lua chon cua ban: ";
}

int main() {
    TreeNode* root = nullptr;
    readFromFile(root, "C:/DSA/DisneyList.TXT");

    if (root == nullptr) {
        cout << "The tree is empty." << endl;
        return 0;
    }

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            int count;
            cout << "Nhap vao so luong muon xuat thong tin: ";
            cin >> count;
            inorderTraversal(root, count);
            break;
        }
        case 2: {
            int subChoice;
            cout << "Nhap lua chon de in ra theo tieu chi (1.Open_Price, 2. Close_Price, 3.Volume): ";
            cin >> subChoice;
            switch (subChoice)
            {
            case 1: {
                Disney maxOpen = findMax(root, "Open_Price");
                cout << "Ngay co Open Price cao nhat:\n";
                cout << "Date: " << maxOpen.Date
                    << ", Open Price: " << maxOpen.Open_Price << endl;
                break;
            }
            case 2: {
                Disney maxClose = findMax(root, "Close_Price");
                cout << "Ngay co Close Price cao nhat:\n";
                cout << "Date: " << maxClose.Date
                    << ", Close Price: " << maxClose.Close_Price << endl;
                break;
            }
            case 3: {
                Disney maxVolume = findMax(root, "Volume");
                cout << "Ngay co Volume lon nhat:\n";
                cout << "Date: " << maxVolume.Date
                    << ", Volume: " << maxVolume.Volume << endl;
                break;
            }
            }
            break;
        }
        case 3: {
            string date;
            float open, high, low, close;
            int volume;

            cout << "Nhap ngay (yyyy-mm-dd): ";
            cin >> date;
            cout << "Nhap gia Open Price: ";
            cin >> open;
            cout << "Nhap gia High Price: ";
            cin >> high;
            cout << "Nhap gia Low Price: ";
            cin >> low;
            cout << "Nhap gia Close Price: ";
            cin >> close;
            cout << "Nhap luong (Volume): ";
            cin >> volume;
            Disney newDisney = { date, open, high, low, close, volume };
            root = insert(root, newDisney);
            writeFile(root, "C:/DSA/DisneyList.TXT");
            cout << "Thong tin da duoc them vao cay va ghi vao file";
            break;
        }
        case 4:
        {
            int year, month, day;
            cout << "Nhap nam: ";
            cin >> year;
            cout << "Nhap thang: ";
            cin >> month;
            cout << "Nhap ngay: ";
            cin >> day;
            root = deleteNodeByDate(root, year, month, day);
            writeFile(root, "C:/DSA/DisneyList.TXT");
            cout << "Node voi ngay " << year << "-" << month << "-" << day << "da duoc xoa va ghi lai vao file.\n";
            break;

        }
        case 5:
            cout << "Thoat chuong trinh." << endl;
            break;
        default:
            cout << "Lua chon khong hop le. Vui long thu lai." << endl;
        }
        } while (choice != 5);

        return 0;
    }

