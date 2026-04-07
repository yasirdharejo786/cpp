#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

// Task class to represent individual tasks
class Task {
private:
    int id;
    string title;
    string description;
    string category;
    bool completed;
    string createdDate;

public:
    // Constructor
    Task(int taskId, string taskTitle, string taskDesc, string taskCategory) {
        id = taskId;
        title = taskTitle;
        description = taskDesc;
        category = taskCategory;
        completed = false;
        
        // Get current date
        time_t now = time(0);
        tm* ltm = localtime(&now);
    createdDate = to_string(1900 + ltm->tm_year) + "-" + 
        to_string(1 + ltm->tm_mon) + "-" + 
        to_string(ltm->tm_mday);
    }

    // Getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getCategory() const { return category; }
    bool isCompleted() const { return completed; }
    string getCreatedDate() const { return createdDate; }

    // Setters
    void setTitle(string newTitle) { title = newTitle; }
    void setDescription(string newDesc) { description = newDesc; }
    void setCategory(string newCategory) { category = newCategory; }
    void markAsCompleted() { completed = true; }
    void markAsPending() { completed = false; }

    // Display task details
    void display() const {
        cout << "\n\n";
        cout << "Task ID: " << id;
        for(int i = 0; i < 34 - to_string(id).length(); i++) cout << " ";
        cout << "\n";
        
        cout << " Status: " << (completed ? "COMPLETED" : " PENDING");
        for(int i = 0; i < 33 - (completed ? 10 : 8); i++) cout << " ";
        cout << "\n";
        
        cout << " Title: " << title;
        for(int i = 0; i < 35 - title.length(); i++) cout << " ";
        cout << "\n";
        
        cout << " Category: " << category;
        for(int i = 0; i < 32 - category.length(); i++) cout << " ";
        cout << "\n";
        
        cout << " Date: " << createdDate;
        for(int i = 0; i < 36 - createdDate.length(); i++) cout << " ";
        cout << "a‚\n";
        
        cout << " Description: " << description;
        for(int i = 0; i < 28 - description.length(); i++) cout << " ";
        cout << "\n";
        cout << "\n\n";
    }

    // Convert task to string for file storage
    string toFileString() const {
        return to_string(id) + "|" + title + "|" + description + "|" + 
               category + "|" + (completed ? "1" : "0") + "|" + createdDate;
    }
};

// TaskManager class to handle all task operations
class TaskManager {
private:
    vector<Task> tasks;
    int nextId;
    const string filename = "tasks.txt";

public:
    TaskManager() {
        nextId = 1;
        loadTasksFromFile();
    }

    // Add new task
    void addTask() {
        string title, description, category;
        
        cout << "\n=== ADD NEW TASK ===\n";
        
        cout << "Enter task title: ";
        cin.ignore();  // Buffer clear karo
        getline(cin, title);
        
        cout << "Enter task description: ";
        getline(cin, description);
        
        cout << "Enter task category (or press Enter for 'General'): ";
        getline(cin, category);
        
        if(category.empty()) category = "General";
        
        tasks.emplace_back(nextId++, title, description, category);
        cout << "\n Task added successfully with ID: " << (nextId - 1) << endl;
        
        saveTasksToFile();
    }

    // View all tasks
    void viewAllTasks() const {
        if(tasks.empty()) {
            cout << "\nðŸ“­ No tasks found!\n";
            return;
        }
        
        cout << "\n========== ALL TASKS ==========\n";
        for(const auto& task : tasks) {
            task.display();
        }
    }

    // View pending tasks
    void viewPendingTasks() const {
        bool found = false;
        cout << "\n========== PENDING TASKS ==========\n";
        
        for(const auto& task : tasks) {
            if(!task.isCompleted()) {
                task.display();
                found = true;
            }
        }
        
        if(!found) cout << "\n No pending tasks! All caught up!\n";
    }

    // View completed tasks
    void viewCompletedTasks() const {
        bool found = false;
        cout << "\n========== COMPLETED TASKS ==========\n";
        
        for(const auto& task : tasks) {
            if(task.isCompleted()) {
                task.display();
                found = true;
            }
        }
        
        if(!found) cout << "\n No completed tasks yet!\n";
    }

    // View tasks by category
    void viewTasksByCategory() const {
        string category;
        cout << "\nEnter category to filter: ";
        cin.ignore();
        getline(cin, category);
        
        bool found = false;
        cout << "\n========== TASKS IN CATEGORY: " << category << " ==========\n";
        
        for(const auto& task : tasks) {
            if(task.getCategory() == category) {
                task.display();
                found = true;
            }
        }
        
        if(!found) cout << "\n No tasks found in category '" << category << "'\n";
    }

    // Mark task as completed
    void markTaskAsCompleted() {
        int id;
        cout << "\nEnter Task ID to mark as completed: "
        cin >> id;
        
        for(auto& task : tasks) {
            if(task.getId() == id) {
                if(task.isCompleted()) {
                    cout << "\n Task is already completed!\n";
                } else {
                    task.markAsCompleted();
                    cout << "\n Task marked as completed!\n";
                    saveTasksToFile();
                }
                return;
            }
        }
        cout << "\n Task not found!\n";
    }

    // Mark task as pending
    void markTaskAsPending() {
        int id;
        cout << "\nEnter Task ID to mark as pending: ";
        cin >> id;
        
        for(auto& task : tasks) {
            if(task.getId() == id) {
                if(!task.isCompleted()) {
                    cout << "\n Task is already pending!\n";
                } else {
                    task.markAsPending();
                    cout << "\n Task marked as pending!\n";
                    saveTasksToFile();
                }
                return;
            }
        }
        cout << "\n Task not found!\n";
    }

    // Edit task
    void editTask() {
        int id;
        cout << "\nEnter Task ID to edit: ";
        cin >> id;
        
        for(auto& task : tasks) {
            if(task.getId() == id) {
                string title, description, category;
                int choice;
                
                cout << "\n What would you like to edit?\n";
                cout << "1. Title\n";
                cout << "2. Description\n";
                cout << "3. Category\n";
                cout << "4. Multiple fields\n";
                cout << "Choice: ";
                cin >> choice;
                cin.ignore();  // Buffer clear karo
                
                switch(choice) {
                    case 1:
                        cout << "Enter new title: ";
                        getline(cin, title);
                        task.setTitle(title);
                        break;
                    case 2:
                        cout << "Enter new description: ";
                        getline(cin, description);
                        task.setDescription(description);
                        break;
                    case 3:
                        cout << "Enter new category: ";
                        getline(cin, category);
                        task.setCategory(category);
                        break;
                    case 4:
                        cout << "Enter new title: ";
                        getline(cin, title);
                        cout << "Enter new description: ";
                        getline(cin, description);
                        cout << "Enter new category: ";
                        getline(cin, category);
                        task.setTitle(title);
                        task.setDescription(description);
                        task.setCategory(category);
                        break;
                    default:
                        cout << "\n Invalid choice!\n";
                        return;
                }
                
                cout << "\n Task updated successfully!\n";
                saveTasksToFile();
                return;
            }
        }
        cout << "\n Task not found!\n";
    }

    // Delete task
    void deleteTask() {
        int id;
        cout << "\nEnter Task ID to delete: ";
        cin >> id;
        
        auto it = remove_if(tasks.begin(), tasks.end(),
            [id](const Task& task) { return task.getId() == id; });
        
        if(it != tasks.end()) {
            tasks.erase(it, tasks.end());
            cout << "\n Task deleted successfully!\n";
            saveTasksToFile();
        } else {
            cout << "\nTask not found!\n";
        }
    }

    // Search tasks
    void searchTasks() const {
        string keyword;
        cout << "\nEnter search keyword: ";
        cin.ignore();
        getline(cin, keyword);
        
        bool found = false;
        cout << "\n========== SEARCH RESULTS ==========\n";
        
        for(const auto& task : tasks) {
            if(task.getTitle().find(keyword) != string::npos ||
               task.getDescription().find(keyword) != string::npos ||
               task.getCategory().find(keyword) != string::npos) {
                task.display();
                found = true;
            }
        }
        
        if(!found) cout << "\n No tasks found matching '" << keyword << "'\n";
    }

    // Display statistics
    void displayStatistics() const {
        int total = tasks.size();
        int completed = count_if(tasks.begin(), tasks.end(),
            [](const Task& t) { return t.isCompleted(); });
        int pending = total - completed;
        
        cout << "\n========== STATISTICS ==========\n";
        cout << " Total Tasks: " << total << endl;
        cout << " Completed: " << completed << endl;
        cout << " Pending: " << pending << endl;
        
        if(total > 0) {
            double completionRate = (static_cast<double>(completed) / total) * 100;
            cout << " Completion Rate: " << completionRate << "%\n";
            
            // Display progress bar
            cout << "Progress: [";
            int filled = static_cast<int>(completionRate / 10);
            for(int i = 0; i < 10; i++) {
                cout << (i < filled ? "–" : "–");
            }
            cout << "]\n";
        }
    }

    // Save tasks to file
    void saveTasksToFile() {
        ofstream file(filename);
        if(file.is_open()) {
            // Save nextId first
            file << nextId << endl;
            
            // Save all tasks
            for(const auto& task : tasks) {
                file << task.toFileString() << endl;
            }
            file.close();
        }
    }

    // Load tasks from file
    void loadTasksFromFile() {
        ifstream file(filename);
        if(file.is_open()) {
            string line;
            
            // Load nextId
            if(getline(file, line)) {
                nextId = stoi(line);
            }
            
            // Load tasks
            while(getline(file, line)) {
                size_t pos1 = line.find('|');
                size_t pos2 = line.find('|', pos1 + 1);
                size_t pos3 = line.find('|', pos2 + 1);
                size_t pos4 = line.find('|', pos3 + 1);
                size_t pos5 = line.find('|', pos4 + 1);
                
                if(pos1 != string::npos && pos2 != string::npos && 
                   pos3 != string::npos && pos4 != string::npos && pos5 != string::npos) {
                    
                    int id = stoi(line.substr(0, pos1));
                    string title = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    string description = line.substr(pos2 + 1, pos3 - pos2 - 1);
                    string category = line.substr(pos3 + 1, pos4 - pos3 - 1);
                    bool completed = (line.substr(pos4 + 1, pos5 - pos4 - 1) == "1");
                    string date = line.substr(pos5 + 1);
                    
                    // Create task manually since we need to set all properties
                    Task task(id, title, description, category);
                    if(completed) task.markAsCompleted();
                    
                    // We need to modify the date, so we'll add it to vector
                    tasks.push_back(task);
                }
            }
            file.close();
        }
    }
};

// Display menu
void displayMenu() {
    cout << "\n\n";
    cout << " TO-DO LIST MANAGER           \n";
    cout << "\n\n\n";
    cout << " 1.Add New Task                   \n";
    cout << " 2. View All Tasks                 \n";
    cout << " 3.View Pending Tasks             \n";
    cout << " 4.View Completed Tasks           \n";
    cout << " 5.View Tasks by Category        \n";
    cout << "6.Mark Task as Completed         \n";
    cout << "7.Mark Task as Pending           \n";
    cout << "8.Edit Task                     \n";
    cout << "9.Delete Task                   \n";
    cout << "10.Search Tasks                   \n";
    cout << "11.View Statistics                \n";
    cout << " 0.Exit                           \n";
    cout << "\n\n";
    cout << "Enter your choice: ";
}

int main() {
    TaskManager manager;
    int choice;
    
    cout << "\n";
    cout << "                                                \n";
    cout << " Welcome to CodeAlpha To-Do List Manager!    \n";
    cout << "\n";
    cout << "\n";
    
    do {
        displayMenu();
        cin >> choice;
        
        // Yeh line bahut important hai - buffer clear karegi
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch(choice) {
            case 1:
                manager.addTask();
                break;
            case 2:
                manager.viewAllTasks();
                break;
            case 3:
                manager.viewPendingTasks();
                break;
            case 4:
                manager.viewCompletedTasks();
                break;
            case 5:
                manager.viewTasksByCategory();
                break;
            case 6:
                manager.markTaskAsCompleted();
                break;
            case 7:
                manager.markTaskAsPending();
                break;
            case 8:
                manager.editTask();
                break;
            case 9:
                manager.deleteTask();
                break;
            case 10:
                manager.searchTasks();
                break;
            case 11:
                manager.displayStatistics();
                break;
            case 0:
                cout << "\n‹ Thank you for using To-Do List Manager!\n";
                cout << "   Keep organizing, keep achieving! ðŸš€\n\n";
                break;
            default:
                cout << "\n Invalid choice! Please try again.\n";
        }
        
    } while(choice != 0);
    
    return 0;
}
