#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;


// Task Class
class Task {
private:
    int id;
    string title;
    string description;
    bool completed;
    string priority;   // NEW: Low / Medium / High
    string dueDate;    // NEW: YYYY-MM-DD

public:
    // Constructor
    Task(int id, string title, string description, string priority = "Medium", string dueDate = "None", bool completed = false) {
        this->id = id;
        this->title = title;
        this->description = description;
        this->priority = priority;
        this->dueDate = dueDate;
        this->completed = completed;
    }

    // Mark as completed
    void markCompleted() {
        completed = true;
    }

    // Display task details
    void displayTask() const {
        cout << "Task ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Description: " << description << endl;
        cout << "Priority: " << priority << endl;
        cout << "Due Date: " << dueDate << endl;
        cout << "Status: " << (completed ? "Completed" : "Pending") << endl;
        cout << "----------------------------" << endl;
    }

    // Getters
    int getId() const { return id; }
    bool isCompleted() const { return completed; }
    string getPriority() const { return priority; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getDueDate() const { return dueDate; }

    // Convert to file string
    string toFileString() const {
        return to_string(id) + "|" + title + "|" + description + "|" + priority + "|" + dueDate + "|" + (completed ? "1" : "0");
    }

    // Create task from file string
    static Task fromFileString(const string& line) {
        int id;
        string title, description, priority, dueDate, status;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);
        size_t pos4 = line.find('|', pos3 + 1);
        size_t pos5 = line.find('|', pos4 + 1);

        id = stoi(line.substr(0, pos1));
        title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        description = line.substr(pos2 + 1, pos3 - pos2 - 1);
        priority = line.substr(pos3 + 1, pos4 - pos3 - 1);
        dueDate = line.substr(pos4 + 1, pos5 - pos4 - 1);
        status = line.substr(pos5 + 1);

        bool completed = (status == "1");
        return Task(id, title, description, priority, dueDate, completed);
    }
};


// File Handling Functions
void saveTasks(const vector<Task>& tasks, const string& filename) {
    ofstream fout(filename);
    for (const auto &task : tasks) {
        fout << task.toFileString() << endl;
    }
    fout.close();
}

void loadTasks(vector<Task>& tasks, const string& filename, int &nextId) {
    ifstream fin(filename);
    string line;
    int maxId = 0;
    while (getline(fin, line)) {
        Task t = Task::fromFileString(line);
        tasks.push_back(t);
        if (t.getId() > maxId) maxId = t.getId();
    }
    fin.close();
    nextId = maxId + 1;
}

// ---------------------------
// Sorting Function
// ---------------------------
int priorityValue(const string& p) {
    if (p == "High") return 3;
    if (p == "Medium") return 2;
    return 1; // Low
}

void sortTasks(vector<Task>& tasks) {
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return priorityValue(a.getPriority()) > priorityValue(b.getPriority());
    });
}


// Main Program
int main() {
    vector<Task> tasks;
    int choice;
    int nextId = 1;
    string filename = "tasks.txt";

    // Load tasks at startup
    loadTasks(tasks, filename, nextId);

    while (true) {
        cout << "\n===== TO-DO LIST MANAGER v4 =====" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. View All Tasks (Sorted by Priority)" << endl;
        cout << "3. Mark Task as Completed" << endl;
        cout << "4. Delete Task" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string title, description, priority, dueDate;
            cout << "Enter Task Title: ";
            getline(cin, title);
            cout << "Enter Task Description: ";
            getline(cin, description);
            cout << "Enter Priority (Low/Medium/High): ";
            getline(cin, priority);
            cout << "Enter Due Date (YYYY-MM-DD): ";
            getline(cin, dueDate);

            Task newTask(nextId, title, description, priority, dueDate);
            tasks.push_back(newTask);
            saveTasks(tasks, filename);
            cout << "Task Added Successfully! [ID: " << nextId << "]\n";
            nextId++;
        }
        else if (choice == 2) {
            if (tasks.empty()) {
                cout << "No tasks available.\n";
            } else {
                sortTasks(tasks);
                cout << "\n--- Task List (Sorted by Priority) ---\n";
                for (const auto &task : tasks) {
                    task.displayTask();
                }
            }
        }
        else if (choice == 3) {
            int id;
            cout << "Enter Task ID to mark as completed: ";
            cin >> id;

            bool found = false;
            for (auto &task : tasks) {
                if (task.getId() == id) {
                    task.markCompleted();
                    saveTasks(tasks, filename);
                    cout << "Task " << id << " marked as completed!\n";
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Task not found!\n";
        }
        else if (choice == 4) {
            int id;
            cout << "Enter Task ID to delete: ";
            cin >> id;

            bool found = false;
            for (auto it = tasks.begin(); it != tasks.end(); ++it) {
                if (it->getId() == id) {
                    tasks.erase(it);
                    saveTasks(tasks, filename);
                    cout << "Task " << id << " deleted successfully!\n";
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Task not found!\n";
        }
        else if (choice == 5) {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}