// TYPE-PULSE -->

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib> // random no. generation ke liye 
#include <fstream> // for file I/O
#include <limits>
#include <iomanip>  // for setw
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define BG_WHITE   "\033[47m"
using namespace std;

class user{
protected:
    string name ;
public:
    user (){
        name= "unknown";
    }
    user(string n){
        name = n;
    }
    void setname(string n){
        name = n;
    }
    string getname ()const {
        return name;
    }
};
class Player : public user { //user + performance data
private:
    double wpm;
    double accuracy;

public:
    // Constructors
    Player() : user("unknown"), wpm(0), accuracy(0) {}
    Player(string n) : user(n), wpm(0), accuracy(0) {}
    Player(string n, double w, double a) : user(n), wpm(w), accuracy(a) {}

    // Setters
    void setScore(double w, double a) {
        wpm = w;
        accuracy = a;
    }

    // Getters
    double getWPM() const { return wpm; }
    double getAccuracy() const { return accuracy; }

    // Display function
    void showInfo() const {
        cout <<setw(15) << getname() << setw(10) << fixed << setprecision(1) << wpm 
             << setw(1) << accuracy << "%" ;
    }
};




const int MAX_PLAYERS = 75; // maximum players allowed

class Leaderboard {
private:
    Player players[MAX_PLAYERS];
    int count; // how many players are currently stored

public:
    Leaderboard() : count(0) {}


    void addPlayer(const Player& p) {
         // Check if player already exists
        for (int i = 0; i < count; i++) {
            if (players[i].getname() == p.getname()) {
            // Update only if new WPM is better
                if (p.getWPM() > players[i].getWPM()) {
                    players[i] = p;
                }
                return; 
            }
        }
        // Otherwise add new
        players[count++] = p;
    }

    void showByWPM(const string& currentUser) {
        // Bubble sort by WPM (descending)
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (players[i].getWPM() < players[j].getWPM()) {
                    Player temp = players[i];
                    players[i] = players[j];
                    players[j] = temp;
                }
            }
        }
        cout << YELLOW << "=====================================\n";
        cout << BOLD << "      Leaderboard (Top 10 by WPM)   \n" << RESET;
        cout << YELLOW << "=====================================\n" << RESET;

        // Table header
        cout << left << setw(5)  << "Rank"
            << setw(15) << "Name"
            << setw(10) << "WPM"
            << setw(10) << "Accuracy" << endl;
        cout << "-----------------------------------------\n";

        int limit = min(count, 10);
        for (int i = 0; i < limit; i++) {
            cout << left << setw(5) << (i+1);
            players[i].showInfo();
            if (i == 0) cout << " 🥇";
            else if (i == 1) cout << " 🥈";
            else if (i == 2) cout << " 🥉";

            if (players[i].getname() == currentUser) {
                cout << GREEN << " <-- YOU" << RESET;
            }
            cout<<endl;
        }

        // Highlight current user if outside top 10
        if (!currentUser.empty()) {
            int position = -1;
            for (int i = 0; i < count; i++) {
                if (players[i].getname() == currentUser) {
                    position = i + 1; // 1-based rank
                    break;
                }
            }

            if (position != -1 && position > 10) {
                cout << "\nYour current position: " << position << endl;
                players[position-1].showInfo();
            } 
            else if (position != -1) {
                cout << "\nYou are already in the Top 10 at position " << position << "!\n";
            }
        }
        cout << endl;
    }

    void showByAccuracy(const string& currentUser) {
        // Bubble sort by Accuracy (descending)
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (players[i].getAccuracy() < players[j].getAccuracy()) {
                    Player temp = players[i];
                    players[i] = players[j];
                    players[j] = temp;
                }
            }
        }

        cout <<YELLOW<< "\n===========================================\n";
        cout <<BOLD<< "       Leaderboard (Top 10 by Accuracy)   \n"<<RESET;
        cout <<YELLOW<< "===========================================\n"<<RESET;

        // Table header
        cout << left << setw(5)  << "Rank"
            << setw(15) << "Name"
            << setw(10) << "WPM"
            << setw(10) << "Accuracy" << endl;
        cout << "----------------------------------------\n";

        int limit = min(count, 10);
        for (int i = 0; i < limit; i++) {
            cout << left << setw(5) <<(i+1);
            players[i].showInfo();
            if (i == 0) cout << " 🥇";
            else if (i == 1) cout << " 🥈";
            else if (i == 2) cout << " 🥉";

            if (players[i].getname() == currentUser) {
                cout << GREEN << " <-- YOU" << RESET;
            }
            cout<<endl;
        }
        // Highlight current user if outside top 10
        if (!currentUser.empty()) {
            int position = -1;
            for (int i = 0; i < count; i++) {
                if (players[i].getname() == currentUser) {
                    position = i + 1;
                    break;
                }
            }

            if (position != -1 && position > 10) {
                cout << "\nYour current position: " << position << endl;
                players[position-1].showInfo();
            } 
            else if (position != -1) {
                cout << "\nYou are already in the Top 10 at position " << position << "!\n";
            }
        }
    }
    int getCount() const { 
        return count; 
    }
    Player getPlayer(int i) const { 
        return players[i]; 
    }

};

// Save leaderboard to file
void saveToFile(const Leaderboard& lb) {
    ofstream out("leaderboard.txt"); // open file for writing
    if (!out) {
        cout << "Error opening file for saving!" << endl;
        return;
    }

    for (int i = 0; i < lb.getCount(); i++) {
        Player p = lb.getPlayer(i);
        out << p.getname() << " "
            << p.getWPM() << " "
            << p.getAccuracy() << endl;
    }
    out.close();
}
// Load leaderboard from file
void loadFromFile(Leaderboard& lb) {
    ifstream in("leaderboard.txt"); // open file for reading
    if (!in) {
        cout << "No previous leaderboard found, starting fresh." << endl;
        return;
    }

    string name;
    double wpm, acc;
    while (in >> name >> wpm >> acc) {
        Player p(name, wpm, acc);
        lb.addPlayer(p);
    }
    in.close();
}
class sentence{
    string text;
    string inputs;
    time_t stime;
    time_t etime;
    time_t ttime; //total time taken
public :
    void gen_sen();  //will sentence choose 
    void start();   //show sentene + take input + time dealing
    double calc_wpm();
    double calc_accuracy();
    void customTextMode();
    string getText() const {
        return text;
    }
    void practicemode(){

    cout << "\n=====================================\n";
    cout <<RED<<BOLD<< "           PRACTICE MODE             \n"<<RESET;
    cout << "=====================================\n";

    cout << "This mode will not affect the leaderboard.\n";
    gen_sen();
    start(); // reusing typing logic
    double wpm = calc_wpm();
    double acc = calc_accuracy();

    cout <<RED<< "\nPractice WPM: " << wpm << " | Accuracy: " << acc << "%" << endl<<RESET;
    cout<<"\n";
    if (wpm < 20) {
        cout << "You're just starting out. Focus on accuracy first, speed will come naturally.\n";
    } 
    else if (wpm < 40) {
        cout << "Nice effort! Try short daily practice sessions to build speed.\n";
    } 
    else if (wpm < 60) {
        cout << "Good progress! You’re typing faster than most beginners.\n";
    } 
    else {
        cout << "Excellent! You’re hitting advanced typing speeds.\n";
    }

    // Feedback based on Accuracy
    if (acc < 55) {
        cout << "Work on reducing mistakes. Slow down a little and aim for cleaner typing.\n";
    } 
    else if (acc < 85) {
        cout << "Solid accuracy, but there’s room to polish. Try focusing on tricky words.\n";
    } 
    else {
        cout << "Great accuracy! Keep it up — consistency is key.\n";
    }

    cout << "\nPress Enter to return to menu...";
            cin.get(); // wait for Enter
}
};

void sentence::gen_sen() {
    string sentences[15] = {
        "Practice typing daily to improve speed and accuracy in your keyboard skills",
        "Consistency and patience are the keys to becoming a fast and accurate typist",
        "Typing without looking at the keyboard increases efficiency and reduces mistakes over time",
        "Focus on accuracy first and speed will automatically improve with regular practice sessions",
        "Developing proper finger placement helps in typing faster and avoiding unnecessary errors",
        "Typing tests help measure performance and identify areas where improvement is needed",
        "Avoid rushing while typing as it can lead to more errors and lower accuracy levels",
        "Reading the sentence carefully before typing helps in reducing unnecessary typing mistakes",
        "A good typing speed is useful for students programmers and professionals in daily tasks",
        "Remember: posture, patience, and practice are the keys to success.",
        "Typing tests aren't just fun; they're useful for real-world tasks.",
        "Don't rush! Slow typing, with fewer mistakes, is better.",
        "Always sit comfortably and maintain good posture while typing for better performance",
        "Improving typing skills can save time and increase productivity in many real world tasks",
        "Staying calm and focused during typing tests leads to better results and fewer mistakes"
    };

    int index = rand() % 15; // rand() will choose a random number and %15 will make it in the range 0-14
    text = sentences[index];
}
void sentence::start() {    //used for - show sentene + take input + time dealing
    //gen_sen();
    

    // Countdown using time()
    cout << "\nGet ready! Starting in...\n";
    for (int i = 3; i > 0; i--) {
        cout << i << "...";
        cout.flush();

        time_t start = time(NULL);
        while (difftime(time(NULL), start) < 1) {
            // wait for 1 second
        }
    }
    cout << "GO!\n";
    cout <<"\n"<<YELLOW<< text << endl<<RESET;

    time(&stime);   //start time
    getline(cin, inputs);       //user input taken here
    time(&etime);   //end time

    ttime = difftime(etime, stime); //gives time in seconds
}
double sentence::calc_wpm() {
    int words = 0;
    bool inWord = false;

    for (int i = 0; i < inputs.length(); i++) {
        if (inputs[i] != ' ' && !inWord) {
        // Start of a new word
            words++;
            inWord = true;
        }
        else if (inputs[i] == ' ') {
        // End of a word, reset flag
            inWord = false;
        }
    }

    double time_minutes = ttime / 60.0;
    if(time_minutes == 0 || words == 0){          //exception if no word written and directly enter is pressed
        return 0; }

    return words / time_minutes;    //word per min 
}

double sentence::calc_accuracy() {
    string s1 = text;
    string s2 = inputs;

    string word1 = "", word2 = "";
    int i = 0, j = 0;
    int correct = 0, total = 0;

    while(i < s1.length() || j < s2.length()) { //whichever has shorter length - wha tak chalega

        // word extract from s1
        word1 = "";
        while(i < s1.length() && s1[i] != ' ') {
            word1 += s1[i];
            i++;
        }
        // word extract from s2
        word2 = "";
        while(j < s2.length() && s2[j] != ' ') {
            word2 += s2[j];
            j++;
        }
        if(word1.length() > 0 || word2.length() > 0) {
            total++;
            if(word1 == word2)
                correct++;
        }
        i++; // skip space
        j++;
    }
    if(total == 0) return 0;
    return ((double)correct / total) * 100;
}
void sentence::customTextMode() {
    cout << "\n=====================================\n";
    cout << RED <<BOLD<< "         CUSTOM TEXT MODE\n" << RESET;
    cout << "=====================================\n";

    cout << "Enter/paste the text you want to practice:\n";
    getline(cin, text);   // user provides their own text

    

    // Countdown before starting
    cout << "\nGet ready! Starting in...\n";
    for (int i = 3; i > 0; i--) {
        cout << i << "...";
        cout.flush();
        time_t start = time(NULL);
        while (difftime(time(NULL), start) < 1) { }
    }
    cout << "GO!\n";
    cout << "\nYour chosen text:\n" <<YELLOW<< text << endl<<RESET;

    time(&stime);
    getline(cin, inputs);
    time(&etime);

    ttime = difftime(etime, stime);

    double wpm = calc_wpm();
    double acc = calc_accuracy();

    cout <<RED<< "\nCustom WPM: " << wpm << " | Accuracy: " << acc << "%" << endl<<RESET;
    cout << "\nPress Enter to continue...";
            cin.get();
}

void multiplayerMode() {

    string p1, p2;
    cout <<RED<< "=====================================\n";
    cout << RED<<BOLD << "        MULTIPLAYER MODE\n" << RESET;
    cout <<RED<< "=====================================\n" << RESET;
    cout << "Player 1 name: ";
    getline(cin, p1);
    cout << "Player 2 name: ";
    getline(cin, p2);

    sentence s;
    // SAME sentence for both players
    s.gen_sen();

    // PLAYER 1
    cout << "\n-------------------------------------\n";
    cout <<CYAN<< p1 << "'s Turn\n"<<RESET;
    cout << "-------------------------------------\n";

    s.start();

    double wpm1 = s.calc_wpm();
    double acc1 = s.calc_accuracy();
    cout << "\nPress Enter to for continue...";
            cin.get(); // wait for Enter
    // PLAYER 2
    cout << "\n-------------------------------------\n";
    cout <<CYAN<< p2 << "'s Turn\n"<<RESET;
    cout << "-------------------------------------\n";

    s.start();

    double wpm2 = s.calc_wpm();
    double acc2 = s.calc_accuracy();
    // RESULTS
    cout <<GREEN<< "\n=====================================\n";
    cout <<BOLD<<GREEN<< "            FINAL RESULT\n"<<RESET;
    cout <<GREEN<< "=====================================\n"<<RESET;

    cout << p1 << " -> WPM: "
         << wpm1
         << " | Accuracy: "
         << acc1 << "%\n";

    cout << p2 << " -> WPM: "
         << wpm2
         << " | Accuracy: "
         << acc2 << "%\n";
    // WINNER
    if (wpm1 > wpm2) {
        cout <<RED<<BOLD<< "\n🏆 WINNER: " << p1 <<"💫"<< endl<<RESET;
    }
    else if (wpm2 > wpm1) {
        cout <<RED<<BOLD<< "\n🏆 WINNER: " << p2 <<"💫"<< endl<<RESET;
    }
    else {
        cout <<RED<<BOLD<< "\nIt's a tie 💫\n"<<RESET;
    }

    cout << "\nPress Enter to continue...";
    cin.get();
}
int menu() {
    cout <<MAGENTA<<BOLD<< "\n|----  MENU  ----|\n"<<RESET;
    cout << "\n1. Take Typing Test\n";
    cout << "2. Show Leaderboard\n";
    cout << "3. Practice Test\n";
    cout << "4. Multiplayer Battle\n";
    cout << "5. Custom Text Mode\n";
    cout << "6. Exit\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline

    return choice;
}
int main () {
system("clear");

cout << CYAN
     << ".=====================================.\n";
cout << BG_WHITE 
     << "|                                     |"
     << RESET << "\n";
cout << BG_WHITE << CYAN << BOLD
     << "|          TYPING PULSE               |"
     << RESET << "\n";
cout << BG_WHITE 
     << "|                                     |"
     << RESET << "\n";
cout << CYAN
     << ".=====================================.\n\n"
     << RESET;

    srand(time(0)); // seed random generator
    Leaderboard lb;
    loadFromFile(lb);   // load saved leaderboard into memory
    // Load scores from file at startup
    loadFromFile(lb);
    string currentUser = "";
   
    while (true) {
        
        int choice = menu();
        
        if (choice == 1) {
            cout << "Enter your name for new test record : ";
            getline(cin, currentUser);

        if (currentUser.empty()) {
            cout << RED << BOLD << "Player name can't be empty" << RESET << endl;
        } else {
            sentence s;
            s.gen_sen();
            s.start();
            double wpm = s.calc_wpm();
            double acc = s.calc_accuracy();

        // Add new player to leaderboard
            Player p(currentUser, wpm, acc);
            lb.addPlayer(p);
            saveToFile(lb);     // save updated leaderboard back to file
            cout << RED << "\nWPM: " << wpm << " | Accuracy: " << acc << "%" << RESET << endl;
            cout << "\n";
            // Feedback based on WPM
            if (wpm < 30) {
                cout << "Suggestion: Try the practice test to improve your speed!" << endl;
            }
            else if (wpm < 50) {
                cout << "Good job! Keep practicing to get even faster." << endl;
            }
            else {
                cout << "Excellent typing speed! You're doing great!" << endl;
            }

            // Optional accuracy feedback
            if (acc < 80) {
                cout << "Tip for better accuracy: Focus on reducing mistakes to improve accuracy." << endl;
            }
            cout << "\nPress Enter to return to menu...";
                    cin.get(); // wait for Enter
        }
        }
        else if (choice == 2) {
            cout<<"Show Leaderboard by :\n a) WPM  b) Accuracy ? "<<endl;
            char x;
            cin>>x;
            cin.ignore(); // clear newline after char input
            if ((x=='a') || (x=='A')){
                lb.showByWPM(currentUser);
            }
            else if ((x == 'b') || (x=='B')) {
                lb.showByAccuracy(currentUser);
            }
            else{
                cout<<"Invalid choice , try again"<<endl;
            }
            cout << "\nPress Enter to return to menu...";
            cin.get(); // wait for Enter
        }
        else if (choice == 3) {
            sentence s;
            s.practicemode();
        }
        else if(choice==4){
            multiplayerMode();
        }
        else if(choice==5){
            sentence s;
            s.customTextMode();
        }
        else if (choice == 6) {
            saveToFile(lb); // save scores before exit
            cout <<RED<< "\nScores saved! Exiting program.\n"<<BOLD<<"Thank you for using TYPE PULSE 😊" << endl<<RESET;
            break;
        }
        else {
            cout << "Invalid choice, try again." << endl;
        }
    }
    return 0;
}