#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <ctime>
#include <sstream>

using namespace std;

// --- Forward Declarations & Enums ---
enum SeatStatus { AVAILABLE, SELECTED, HELD, BOOKED };
enum SeatCategory { STANDARD, PREMIUM, RECLINER, WHEELCHAIR };
enum BookingStatus { CONFIRMED, CANCELLED };

struct Seat {
    int seatId;
    string seatNumber; // e.g., "A1"
    SeatCategory category;
    double price;
    SeatStatus status;
};

struct Screen {
    int screenId;
    string screenName;
    int totalRows;
    int colsPerRow;
    vector<Seat> seats;
};

struct Movie {
    int movieId;
    string title;
    string genre;
    string language;
    double rating;
    int durationMinutes;
    string releaseDate;
    string synopsis;
};

struct Cinema {
    int cinemaId;
    string name;
    string location;
    vector<Screen> screens;
};

struct Showtime {
    int showtimeId;
    int movieId;
    int cinemaId;
    int screenId;
    string date;
    string timeSlot;
    string timeCategory; // MORNING, AFTERNOON, EVENING, NIGHT
};

struct FoodItem {
    int foodId;
    string name;
    string description;
    double price;
    string category; // Popcorn, Beverage, Snack, Combo
};

struct FoodOrder {
    int foodId;
    int quantity;
};

struct Coupon {
    string code;
    double discountPercentage;
    double flatDiscount;
    double minOrderValue;
};

struct Booking {
    int bookingId;
    int userId;
    int showtimeId;
    vector<int> seatIds;
    vector<FoodOrder> foodOrders;
    string couponUsed;
    double ticketTotal;
    double foodTotal;
    double discountAmount;
    double convenienceFee;
    double finalTotal;
    BookingStatus status;
    string bookingDate;
};

struct User {
    int userId;
    string name;
    string email;
    string phone;
    string role; // "CUSTOMER" or "ADMIN"
};

// --- In-Memory Database / Data Store ---
class Database {
public:
    vector<User> users;
    vector<Movie> movies;
    vector<Cinema> cinemas;
    vector<Showtime> showtimes;
    vector<FoodItem> foodItems;
    vector<Coupon> coupons;
    vector<Booking> bookings;
    int nextBookingId = 1001;

    void seedData() {
        // 1. Users
        users.push_back({1, "Alice Smith", "alice@example.com", "9876543210", "CUSTOMER"});
        users.push_back({2, "Admin User", "admin@showtime.com", "9123456789", "ADMIN"});

        // 2. Movies (8 items)
        movies.push_back({1, "Interstellar", "Sci-Fi / Drama", "English", 8.7, 169, "2014-11-07", "A team of explorers travel through a wormhole in space in an attempt to ensure humanity's survival."});
        movies.push_back({2, "Dune: Part Two", "Sci-Fi / Adventure", "English", 8.5, 166, "2024-03-01", "Paul Atreides unites with Chani and the Fremen while seeking revenge against the conspirators who destroyed his family."});
        movies.push_back({3, "Oppenheimer", "Biography / Drama", "English", 8.9, 180, "2023-07-21", "The story of American scientist J. Robert Oppenheimer and his role in the development of the atomic bomb."});
        movies.push_back({4, "The Dark Knight", "Action / Crime", "English", 9.0, 152, "2008-07-18", "When the menace known as the Joker wreaks havoc and chaos on the people of Gotham, Batman must accept one of the greatest psychological and physical tests."});
        movies.push_back({5, "Spider-Man: Across the Spider-Verse", "Animation / Action", "English", 8.7, 140, "2023-06-02", "Miles Morales catapults across the Multiverse, where he encounters a team of Spider-People charged with protecting its very existence."});
        movies.push_back({6, "Inception", "Sci-Fi / Action", "English", 8.8, 148, "2010-07-16", "A thief who steals corporate secrets through the use of dream-sharing technology is given the inverse task of planting an idea."});
        movies.push_back({7, "Avatar: The Way of Water", "Sci-Fi / Action", "English", 7.6, 192, "2022-12-16", "Jake Sully lives with his newfound family formed on the extrasolar moon Pandora."});
        movies.push_back({8, "Parasite", "Thriller / Drama", "Korean", 8.5, 132, "2019-05-30", "Greed and class discrimination threaten the newly formed symbiotic relationship between the wealthy Park family and the destitute Kim clan."});

        // 3. Cinemas & Screens & Seats
        // Create 4 Cinemas
        vector<string> cinemaNames = {"PVR ICON: Phoenix Mills", "Cinepolis: Westend", "IMAX Nexus Mall", "INOX Leisure: Megaplex"};
        vector<string> locations = {"Lower Parel", "Aundh", "Koregaon Park", "Viman Nagar"};

        for (int c = 0; c < 4; ++c) {
            Cinema cin;
            cin.cinemaId = c + 1;
            cin.name = cinemaNames[c];
            cin.location = locations[c];

            // 2 Screens per cinema
            for (int s = 0; s < 2; ++s) {
                Screen scr;
                scr.screenId = (c * 2) + s + 1;
                scr.screenName = (s == 0) ? "IMAX Screen 1" : "Audi 2 (Dolby Atmos)";
                scr.totalRows = 5; // A to E
                scr.colsPerRow = 8;  // 1 to 8

                // Generate Seats
                for (int r = 0; r < scr.totalRows; ++r) {
                    char rowChar = 'A' + r;
                    for (int col = 1; col <= scr.colsPerRow; ++col) {
                        Seat seat;
                        seat.seatId = (scr.screenId * 100) + (r * scr.colsPerRow) + col;
                        seat.seatNumber = string(1, rowChar) + to_string(col);
                        
                        if (r == 0) {
                            seat.category = RECLINER;
                            seat.price = 450.0;
                        } else if (r == 1 || r == 2) {
                            seat.category = PREMIUM;
                            seat.price = 300.0;
                        } else {
                            seat.category = STANDARD;
                            seat.price = 200.0;
                        }
                        
                        // Mark some as pre-booked for realism
                        if ((seat.seatId % 7 == 0) || (seat.seatId % 13 == 0)) {
                            seat.status = BOOKED;
                        } else {
                            seat.status = AVAILABLE;
                        }
                        scr.seats.push_back(seat);
                    }
                }
                cin.screens.push_back(scr);
            }
            cinemas.push_back(cin);
        }

        // 4. Showtimes
        int stId = 1;
        for (const auto& mov : movies) {
            for (const auto& cin : cinemas) {
                for (const auto& scr : cin.screens) {
                    showtimes.push_back({stId++, mov.movieId, cin.cinemaId, scr.screenId, "2026-06-10", "10:30 AM", "MORNING"});
                    showtimes.push_back({stId++, mov.movieId, cin.cinemaId, scr.screenId, "2026-06-10", "04:15 PM", "EVENING"});
                    showtimes.push_back({stId++, mov.movieId, cin.cinemaId, scr.screenId, "2026-06-10", "09:30 PM", "NIGHT"});
                }
            }
        }

        // 5. Food Items
        foodItems.push_back({1, "Large Butter Popcorn", "Freshly popped warm buttery popcorn (Large)", 280.0, "Popcorn"});
        foodItems.push_back({2, "Caramel Popcorn", "Sweet crunchy caramel coated popcorn", 320.0, "Popcorn"});
        foodItems.push_back({3, "Coke / Pepsi (Large)", "Chilled carbonated soft drink 750ml", 180.0, "Beverage"});
        foodItems.push_back({4, "Nachos with Cheese & Salsa", "Crispy corn nachos served with warm melted cheese dip", 290.0, "Snack"});
        foodItems.push_back({5, "Movie Combo (Popcorn + 2 Drinks)", "Large Popcorn + 2 Large Soft Drinks (Save 15%)", 550.0, "Combo"});

        // 6. Coupons
        coupons.push_back({"SHOWTIME50", 0.0, 50.0, 300.0}); // Flat 50 off on min 300
        coupons.push_back({"STUDENT20", 20.0, 0.0, 200.0});  // 20% off
    }
};

// --- Application Controller / UI System ---
class ShowtimeApp {
private:
    Database db;
    User currentUser;

    void clearScreen() {
        #if defined(_WIN32)
            system("cls");
        #else
            system("clear");
        #endif
    }

    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    const Movie* findMovieById(int id) {
        for (const auto& m : db.movies) if (m.movieId == id) return &m;
        return nullptr;
    }

    const Cinema* findCinemaById(int id) {
        for (const auto& c : db.cinemas) if (c.cinemaId == id) return &c;
        return nullptr;
    }

    Cinema* findCinemaByIdNonConst(int id) {
        for (auto& c : db.cinemas) if (c.cinemaId == id) return &c;
        return nullptr;
    }

    const Showtime* findShowtimeById(int id) {
        for (const auto& s : db.showtimes) if (s.showtimeId == id) return &s;
        return nullptr;
    }

public:
    ShowtimeApp() {
        db.seedData();
        currentUser = db.users[0]; // Default Customer
    }

    void run() {
        int choice = 0;
        do {
            clearScreen();
            cout << "========================================================\n";
            cout << "            SHOWTIME SOLUTIONS — CINEMA PLATFORM        \n";
            cout << "            \"Your Movie. Your Showtime. Your Experience.\"\n";
            cout << "========================================================\n";
            cout << "Logged in as: " << currentUser.name << " (" << currentUser.role << ")\n";
            cout << "--------------------------------------------------------\n";
            cout << " 1. Browse Movies & Discover\n";
            cout << " 2. Global Search (Movies, Cinemas, Genres)\n";
            cout << " 3. View My Bookings & Digital Tickets\n";
            cout << " 4. Offers & Promotions\n";
            if (currentUser.role == "ADMIN") {
                cout << " 5. Admin Analytics & Management Dashboard\n";
                cout << " 6. Switch User Role (Customer / Admin)\n";
            } else {
                cout << " 5. Switch User Role (Customer / Admin)\n";
            }
            cout << " 0. Exit Platform\n";
            cout << "========================================================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: browseMovies(); break;
                case 2: globalSearch(); break;
                case 3: viewMyBookings(); break;
                case 4: viewOffers(); break;
                case 5:
                    if (currentUser.role == "ADMIN") {
                        adminDashboard();
                    } else {
                        switchRole();
                    }
                    break;
                case 6:
                    if (currentUser.role == "ADMIN") {
                        switchRole();
                    } else {
                        cout << "Invalid choice. Exiting...\n";
                    }
                    break;
                case 0: cout << "Thank you for using Showtime Solutions!\n"; break;
                default: cout << "Invalid option. Try again.\n"; pause();
            }
        } while (choice != 0);
    }

private:
    void switchRole() {
        if (currentUser.role == "CUSTOMER") {
            currentUser = db.users[1]; // Admin
            cout << "Switched to ADMIN mode.\n";
        } else {
            currentUser = db.users[0]; // Customer
            cout << "Switched to CUSTOMER mode.\n";
        }
        pause();
    }

    void browseMovies() {
        clearScreen();
        cout << "========================================================\n";
        cout << "                     NOW SHOWING & MOVIES               \n";
        cout << "========================================================\n";
        for (const auto& m : db.movies) {
            cout << "[" << m.movieId << "] " << m.title << " (" << m.language << ")\n";
            cout << "    Genre: " << m.genre << " | Rating: " << m.rating << "/10 | Duration: " << m.durationMinutes << " mins\n";
            cout << "    Synopsis: " << m.synopsis.substr(0, 90) << "...\n";
            cout << "--------------------------------------------------------\n";
        }
        cout << "\nEnter Movie ID to book tickets (or 0 to return): ";
        int mId;
        cin >> mId;
        if (mId > 0 && findMovieById(mId)) {
            selectCinemaAndShowtime(mId);
        }
    }

    void selectCinemaAndShowtime(int movieId) {
        clearScreen();
        const Movie* mov = findMovieById(movieId);
        cout << "=== SELECT CINEMA & SHOWTIME FOR: " << mov->title << " ===\n\n";

        vector<const Showtime*> matchingShows;
        for (const auto& st : db.showtimes) {
            if (st.movieId == movieId) matchingShows.push_back(&st);
        }

        if (matchingShows.empty()) {
            cout << "No showtimes available currently.\n";
            pause();
            return;
        }

        int idx = 1;
        map<int, const Showtime*> showMap;
        for (const auto& st : matchingShows) {
            const Cinema* cin = findCinemaById(st->cinemaId);
            cout << "[" << idx << "] " << cin->name << " (" << cin->location << ")\n";
            cout << "    Date: " << st->date << " | Time: " << st->timeSlot << " [" << st->timeCategory << "]\n";
            cout << "--------------------------------------------------------\n";
            showMap[idx++] = st;
        }

        cout << "\nSelect Showtime index (or 0 to cancel): ";
        int choice;
        cin >> choice;
        if (showMap.find(choice) != showMap.end()) {
            startSeatBooking(*showMap[choice]);
        }
    }

    void startSeatBooking(const Showtime& showtime) {
        Cinema* cinema = findCinemaByIdNonConst(showtime.cinemaId);
        Screen* screen = nullptr;
        for (auto& scr : cinema->screens) {
            if (scr.screenId == showtime.screenId) {
                screen = &scr;
                break;
            }
        }

        vector<int> selectedSeatIds;
        char action = ' ';

        do {
            clearScreen();
            cout << "========================================================\n";
            cout << "               REAL-TIME SEAT BOOKING                   \n";
            cout << "========================================================\n";
            cout << "Cinema: " << cinema->name << " | Screen: " << screen->screenName << "\n";
            cout << "Showtime: " << showtime.date << " at " << showtime.timeSlot << "\n\n";
            cout << "                    [ SCREEN THIS WAY ]                 \n";
            cout << "--------------------------------------------------------\n";

            // Render Seat Map
            for (int r = 0; r < screen->totalRows; ++r) {
                char rowChar = 'A' + r;
                cout << "Row " << rowChar << " | ";
                for (int col = 1; col <= screen->colsPerRow; ++col) {
                    // Find seat
                    Seat* currentSeat = nullptr;
                    for (auto& s : screen->seats) {
                        if (s.seatNumber == string(1, rowChar) + to_string(col)) {
                            currentSeat = &s;
                            break;
                        }
                    }

                    if (currentSeat) {
                        string symbol = "[ ]";
                        if (currentSeat->status == BOOKED) symbol = "[X]";
                        else if (currentSeat->status == HELD) symbol = "[H]";
                        else if (currentSeat->status == SELECTED) symbol = "[S]";
                        else {
                            if (currentSeat->category == RECLINER) symbol = "{ }";
                            else if (currentSeat->category == PREMIUM) symbol = "( )";
                        }
                        cout << currentSeat->seatNumber << symbol << " ";
                    }
                }
                cout << "\n";
            }

            cout << "\nLegend: [ ] Available | ( ) Premium | { } Recliner | [S] Selected | [X] Booked\n";
            cout << "--------------------------------------------------------\n";
            cout << "Currently Selected Seats: ";
            if (selectedSeatIds.empty()) cout << "None\n";
            else {
                for (int sId : selectedSeatIds) {
                    for (const auto& s : screen->seats) {
                        if (s.seatId == sId) cout << s.seatNumber << " ";
                    }
                }
                cout << "\n";
            }

            cout << "\nOptions:\n";
            cout << " 1. Select / Deselect a Seat (e.g. A1)\n";
            cout << " 2. Proceed to Food & Beverages Checkout\n";
            cout << " 0. Cancel and Return\n";
            cout << "Enter choice: ";
            cin >> action;

            if (action == '1') {
                cout << "Enter seat number to toggle (e.g., A3): ";
                string sNum;
                cin >> sNum;
                transform(sNum.begin(), sNum.end(), sNum.begin(), ::toupper);

                Seat* targetSeat = nullptr;
                for (auto& s : screen->seats) {
                    if (s.seatNumber == sNum) {
                        targetSeat = &s;
                        break;
                    }
                }

                if (!targetSeat) {
                    cout << "Invalid seat number!\n";
                    pause();
                } else if (targetSeat->status == BOOKED || targetSeat->status == HELD) {
                    cout << "Error: That seat is already booked or held by another user!\n";
                    pause();
                } else {
                    // Toggle selection
                    if (targetSeat->status == SELECTED) {
                        targetSeat->status = AVAILABLE;
                        selectedSeatIds.erase(remove(selectedSeatIds.begin(), selectedSeatIds.end(), targetSeat->seatId), selectedSeatIds.end());
                    } else {
                        targetSeat->status = SELECTED;
                        selectedSeatIds.push_back(targetSeat->seatId);
                    }
                }
            } else if (action == '2') {
                if (selectedSeatIds.empty()) {
                    cout << "Please select at least one seat before proceeding!\n";
                    pause();
                } else {
                    // Mark seats as HELD temporarily during checkout simulation
                    for (int sId : selectedSeatIds) {
                        for (auto& s : screen->seats) {
                            if (s.seatId == sId) s.status = HELD;
                        }
                    }
                    checkoutFlow(showtime, screen, selectedSeatIds);
                    break;
                }
            }
        } while (action != '0');
    }

    void checkoutFlow(const Showtime& showtime, Screen* screen, const vector<int>& seatIds) {
        clearScreen();
        cout << "========================================================\n";
        cout << "            CHECKOUT & FOOD & BEVERAGES                 \n";
        cout << "========================================================\n";

        double ticketTotal = 0.0;
        cout << "Selected Seats Summary:\n";
        for (int sId : seatIds) {
            for (const auto& s : screen->seats) {
                if (s.seatId == sId) {
                    cout << " - Seat " << s.seatNumber << " (" << (s.category == RECLINER ? "Recliner" : s.category == PREMIUM ? "Premium" : "Standard") << ") : Rs. " << s.price << "\n";
                    ticketTotal += s.price;
                }
            }
        }
        cout << "Ticket Subtotal: Rs. " << ticketTotal << "\n";
        cout << "--------------------------------------------------------\n";

        // Food Ordering
        cout << "Available F&B Combos & Snacks:\n";
        for (const auto& f : db.foodItems) {
            cout << "[" << f.foodId << "] " << f.name << " - Rs. " << f.price << "\n    " << f.description << "\n";
        }
        cout << "--------------------------------------------------------\n";
        cout << "Would you like to add food items? (y/n): ";
        char addFood;
        cin >> addFood;

        vector<FoodOrder> foodOrders;
        double foodTotal = 0.0;

        if (addFood == 'y' || addFood == 'Y') {
            char more = 'y';
            while (more == 'y' || more == 'Y') {
                int fId, qty;
                cout << "Enter Food ID: ";
                cin >> fId;
                cout << "Enter Quantity: ";
                cin >> qty;

                for (const auto& f : db.foodItems) {
                    if (f.foodId == fId) {
                        foodOrders.push_back({fId, qty});
                        foodTotal += (f.price * qty);
                    }
                }
                cout << "Add another food item? (y/n): ";
                cin >> more;
            }
        }

        // Coupon Application
        double discountAmount = 0.0;
        string appliedCoupon = "";
        cout << "\nEnter Coupon Code (or type NONE): ";
        string couponCode;
        cin >> couponCode;
        transform(couponCode.begin(), couponCode.end(), couponCode.begin(), ::toupper);

        double subTotal = ticketTotal + foodTotal;
        for (const auto& c : db.coupons) {
            if (c.code == couponCode && subTotal >= c.minOrderValue) {
                appliedCoupon = c.code;
                if (c.discountPercentage > 0) {
                    discountAmount = subTotal * (c.discountPercentage / 100.0);
                } else {
                    discountAmount = c.flatDiscount;
                }
                cout << "Coupon applied successfully! Saved Rs. " << discountAmount << "\n";
                break;
            }
        }

        if (appliedCoupon.empty() && couponCode != "NONE") {
            cout << "Invalid coupon or minimum order value not met. No discount applied.\n";
        }

        double convenienceFee = 35.0;
        double finalTotal = subTotal - discountAmount + convenienceFee;

        cout << "\n========================================================\n";
        cout << "                    SMART BILLING SUMMARY               \n";
        cout << "========================================================\n";
        cout << " Tickets Subtotal   : Rs. " << ticketTotal << "\n";
        cout << " Food & Beverages   : Rs. " << foodTotal << "\n";
        cout << " Discount (" << (appliedCoupon.empty() ? "None" : appliedCoupon) << ") : -Rs. " << discountAmount << "\n";
        cout << " Convenience Fee    : Rs. " << convenienceFee << "\n";
        cout << "--------------------------------------------------------\n";
        cout << " FINAL PAYABLE      : Rs. " << finalTotal << "\n";
        cout << "========================================================\n";

        cout << "Proceed to secure payment? (y/n): ";
        char pay;
        cin >> pay;

        if (pay == 'y' || pay == 'Y') {
            // Confirm booking -> Mark seats as BOOKED
            for (int sId : seatIds) {
                for (auto& s : screen->seats) {
                    if (s.seatId == sId) s.status = BOOKED;
                }
            }

            Booking newBooking;
            newBooking.bookingId = db.nextBookingId++;
            newBooking.userId = currentUser.userId;
            newBooking.showtimeId = showtime.showtimeId;
            newBooking.seatIds = seatIds;
            newBooking.foodOrders = foodOrders;
            newBooking.couponUsed = appliedCoupon;
            newBooking.ticketTotal = ticketTotal;
            newBooking.foodTotal = foodTotal;
            newBooking.discountAmount = discountAmount;
            newBooking.convenienceFee = convenienceFee;
            newBooking.finalTotal = finalTotal;
            newBooking.status = CONFIRMED;
            newBooking.bookingDate = "2026-06-08";

            db.bookings.push_back(newBooking);

            cout << "\nPayment Successful! Booking confirmed.\n";
            renderDigitalTicket(newBooking);
        } else {
            // Release seats back to available
            for (int sId : seatIds) {
                for (auto& s : screen->seats) {
                    if (s.seatId == sId) s.status = AVAILABLE;
                }
            }
            cout << "Payment cancelled. Your seat reservations have been released.\n";
            pause();
        }
    }

    void renderDigitalTicket(const Booking& booking) {
        clearScreen();
        const Showtime* st = findShowtimeById(booking.showtimeId);
        const Movie* mov = findMovieById(st->movieId);
        const Cinema* cin = findCinemaById(st->cinemaId);
        Cinema* cinemaObj = findCinemaByIdNonConst(st->cinemaId);
        Screen* scr = nullptr;
        for (auto& s : cinemaObj->screens) if (s.screenId == st->screenId) scr = &s;

        cout << "\n========================================================\n";
        cout << "              SHOWTIME SOLUTIONS — E-TICKET             \n";
        cout << "========================================================\n";
        cout << " Booking ID : #" << booking.bookingId << "           Status: CONFIRMED\n";
        cout << " Movie      : " << mov->title << " (" << mov->language << ")\n";
        cout << " Cinema     : " << cin->name << " (" << cin->location << ")\n";
        cout << " Screen     : " << scr->screenName << "\n";
        cout << " Date & Time: " << st->date << " | " << st->timeSlot << "\n";
        cout << " Seats      : ";
        for (int sId : booking.seatIds) {
            for (const auto& s : scr->seats) {
                if (s.seatId == sId) cout << s.seatNumber << " ";
            }
        }
        cout << "\n--------------------------------------------------------\n";
        cout << " Customer   : " << currentUser.name << " (" << currentUser.email << ")\n";
        cout << " Total Paid : Rs. " << booking.finalTotal << " (Incl. Taxes & F&B)\n";
        cout << "========================================================\n";
        cout << "                   [ QR CODE HASH ]                     \n";
        cout << "                 ||| | |||| || | ||                     \n";
        cout << "========================================================\n";
        pause();
    }

    void viewMyBookings() {
        clearScreen();
        cout << "========================================================\n";
        cout << "                   MY BOOKINGS HISTORY                  \n";
        cout << "========================================================\n";

        vector<const Booking*> userBookings;
        for (const auto& b : db.bookings) {
            if (b.userId == currentUser.userId) userBookings.push_back(&b);
        }

        if (userBookings.empty()) {
            cout << "You have no bookings recorded yet.\n";
            pause();
            return;
        }

        int idx = 1;
        map<int, Booking*> bMap;
        for (const auto& b : userBookings) {
            const Showtime* st = findShowtimeById(b->showtimeId);
            const Movie* mov = findMovieById(st->movieId);
            const Cinema* cin = findCinemaById(st->cinemaId);

            cout << "[" << idx << "] Booking ID: #" << b->bookingId << " | " << mov->title << "\n";
            cout << "    Cinema: " << cin->name << " | Date: " << st->date << " at " << st->timeSlot << "\n";
            cout << "    Status: " << (b->status == CONFIRMED ? "CONFIRMED" : "CANCELLED") << " | Total: Rs. " << b->finalTotal << "\n";
            cout << "--------------------------------------------------------\n";
            // Non-const pointer lookup
            for (auto& realB : db.bookings) {
                if (realB.bookingId == b->bookingId) bMap[idx] = &realB;
            }
            idx++;
        }

        cout << "\nEnter booking number to View Ticket or Cancel (0 to return): ";
        int choice;
        cin >> choice;
        if (bMap.find(choice) != bMap.end()) {
            Booking* selBooking = bMap[choice];
            cout << "\n1. View Digital Ticket\n2. Cancel Booking\nChoice: ";
            int action;
            cin >> action;
            if (action == 1) {
                renderDigitalTicket(*selBooking);
            } else if (action == 2 && selBooking->status == CONFIRMED) {
                // Cancel booking and release seats
                selBooking->status = CANCELLED;
                const Showtime* st = findShowtimeById(selBooking->showtimeId);
                Cinema* cin = findCinemaByIdNonConst(st->cinemaId);
                for (auto& scr : cin->screens) {
                    if (scr.screenId == st->screenId) {
                        for (int sId : selBooking->seatIds) {
                            for (auto& s : scr.seats) {
                                if (s.seatId == sId) s.status = AVAILABLE;
                            }
                        }
                    }
                }
                cout << "Booking cancelled successfully. Seats released and refund initiated.\n";
                pause();
            }
        }
    }

    void viewOffers() {
        clearScreen();
        cout << "========================================================\n";
        cout << "              OFFERS & PROMOTIONS                       \n";
        cout << "========================================================\n";
        cout << "1. CODE: SHOWTIME50\n   Flat Rs. 50 OFF on orders above Rs. 300.\n\n";
        cout << "2. CODE: STUDENT20\n   20% OFF on all ticket bookings above Rs. 200.\n\n";
        cout << "3. WEEKEND COMBO SPECIAL\n   Get 15% off when you bundle Large Popcorn with Movie Tickets.\n";
        cout << "========================================================\n";
        pause();
    }

    void globalSearch() {
        clearScreen();
        cout << "========================================================\n";
        cout << "                   GLOBAL SEARCH                        \n";
        cout << "========================================================\n";
        cout << "Enter keyword (Movie title, genre, cinema name): ";
        string query;
        cin >> query;
        transform(query.begin(), query.end(), query.begin(), ::tolower);

        cout << "\n--- Search Results for \"" << query << "\" ---\n\n";
        bool found = false;

        cout << "MOVIES:\n";
        for (const auto& m : db.movies) {
            string titleLower = m.title;
            string genreLower = m.genre;
            transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            transform(genreLower.begin(), genreLower.end(), genreLower.begin(), ::tolower);

            if (titleLower.find(query) != string::npos || genreLower.find(query) != string::npos) {
                cout << " - " << m.title << " [" << m.genre << "] (" << m.language << ")\n";
                found = true;
            }
        }

        cout << "\nCINEMAS:\n";
        for (const auto& c : db.cinemas) {
            string nameLower = c.name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(query) != string::npos) {
                cout << " - " << c.name << " at " << c.location << "\n";
                found = true;
            }
        }

        if (!found) cout << "No matching records found.\n";
        pause();
    }

    void adminDashboard() {
        clearScreen();
        int totalUsers = db.users.size();
        int totalBookings = db.bookings.size();
        double totalRevenue = 0.0;
        int confirmedBookings = 0;

        for (const auto& b : db.bookings) {
            if (b.status == CONFIRMED) {
                totalRevenue += b.finalTotal;
                confirmedBookings++;
            }
        }

        cout << "========================================================\n";
        cout << "            ADMIN ANALYTICS & MANAGEMENT                \n";
        cout << "========================================================\n";
        cout << " TOTAL REGISTERED USERS  : " << totalUsers << "\n";
        cout << " TOTAL CONFIRMED BOOKINGS: " << confirmedBookings << "\n";
        cout << " TOTAL PLATFORM REVENUE  : Rs. " << totalRevenue << "\n";
        cout << " ACTIVE CINEMAS & SCREENS: " << db.cinemas.size() << " Cinemas\n";
        cout << "========================================================\n";
        cout << " 1. View All System Bookings\n";
        cout << " 2. Add New Movie\n";
        cout << " 0. Back to Main Menu\n";
        cout << "========================================================\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            clearScreen();
            cout << "=== ALL SYSTEM BOOKINGS ===\n\n";
            for (const auto& b : db.bookings) {
                cout << "Booking #" << b.bookingId << " | User ID: " << b.userId << " | Total: Rs. " << b.finalTotal << " | Status: " << (b.status == CONFIRMED ? "CONFIRMED" : "CANCELLED") << "\n";
            }
            pause();
        } else if (choice == 2) {
            Movie newMov;
            newMov.movieId = db.movies.size() + 1;
            cout << "Enter Movie Title: ";
            cin.ignore();
            getline(cin, newMov.title);
            cout << "Enter Genre: ";
            getline(cin, newMov.genre);
            cout << "Enter Language: ";
            getline(cin, newMov.language);
            cout << "Enter IMDb Rating: ";
            cin >> newMov.rating;
            cout << "Enter Duration (minutes): ";
            cin >> newMov.durationMinutes;
            newMov.releaseDate = "2026-06-15";
            newMov.synopsis = "Newly added blockbuster feature film.";

            db.movies.push_back(newMov);
            cout << "Movie added successfully!\n";
            pause();
        }
    }
};

int main() {
    ShowtimeApp app;
    app.run();
    return 0;
}
