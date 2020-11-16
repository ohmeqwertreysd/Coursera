#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <cctype>
#include <map>
#include <exception>
#include <stdexcept>
#include <limits>

struct Day
{
    int value;
    explicit Day(int v) : value(v) {}
};

struct Month
{
    int value;
    explicit Month(int v) : value(v) {}
};

struct Year
{
    int value;
    explicit Year(int v) : value(v) {}
};

class Date {
private:
    Day day;
    Month month;
    Year year;
public:
    Date() : day(0), month(0), year(0) { }
    Date(Day d, Month m, Year y) : day(d.value), month(m.value), year(y.value) { }
    int GetYear() const
    {
        return year.value;
    }

    int GetMonth() const
    {
        return month.value;
    }

    int GetDay() const
    {
        return day.value;
    }

    bool operator<(const Date& rhs) const
    {
        if (year.value < rhs.GetYear())
            return true;
        else if (year.value > rhs.GetYear())
            return false;
        else
        {
            if (month.value < rhs.GetMonth())
                return true;
            else if (month.value > rhs.GetMonth())
                return false;
            else
            {
                if (day.value < rhs.GetDay())
                    return true;
                else if (day.value > rhs.GetDay())
                    return false;
                else
                    return false;
            }
        }
    }
};

class Database {
private:
    std::map<Date, std::set<std::string>> events;
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        events[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const std::string& event)
    {
        if (events[date].count(event) && events.count(date))
        {
            events[date].erase(event);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date& date)
    {
        if (events.count(date) == 1)
        {
            int count = events.at(date).size();
            events.erase(date);
            return count;
        }
        else
            return 0;
    }

    std::set<std::string> Find(const Date& date) const
    {
        if (events.count(date))
            return events.at(date);
        else
            return {};
    }

    void Print() const
    {
        for (const auto& [date, eventsSet] : events)
        {
            for (const auto& e : eventsSet)
            {
                std::cout << std::setw(4) << std::setfill('0') << date.GetYear()
                    << '-' << std::setw(2) << date.GetMonth() << '-' << std::setw(2) << date.GetDay();
                std::cout << ' ' << e << std::endl;
            }
        }
    }
};

bool frontValidation(std::stringstream& stream)
{
    int count = 0;
    size_t len = stream.str().length();
    for (size_t i = stream.tellg(); i < len; i++)
    {
        if (!isdigit(stream.str()[i]))
            count++;
        else
            break;
    }
    if (count >= 2)
        return false;
    return true;
}

bool EnsureNextSymbolAndSkip(std::stringstream& stream)
{
    if (stream.peek() != '-')
        return false;
    stream.ignore(1);
    return true;
}

void monthValid(const int& month)
{
    if (month < 1 || month > 12)
        throw std::runtime_error("Month value is invalid: " + std::to_string(month));
}

void dayValid(const int& day)
{
    if (day < 1 || day > 31)
        throw std::runtime_error("Day value is invalid: " + std::to_string(day));
}

Date getDate(const std::string& command)
{
    std::stringstream stream(command);
    int max = std::numeric_limits<int>::max();
    int date[3] = { max, max, max };
    if (std::count_if(command.begin(), command.end(), [](const auto& c) {return !isdigit(c) && c != '-' && c != '+'; }))
        throw std::runtime_error("Wrong date format: " + command);
    for (int i = 0; i < 3; i++)
    {
        if (!frontValidation(stream))
            throw std::runtime_error("Wrong date format: " + command);

        stream >> date[i];

        if (i != 2 && !EnsureNextSymbolAndSkip(stream))
            throw std::runtime_error("Wrong date format: " + command);
    }
    if (date[0] == max || date[1] == max || date[2] == max)
        throw std::runtime_error("Wrong date format: " + command);

    monthValid(date[1]);
    dayValid(date[2]);
    return Date(Day(date[2]), Month(date[1]), Year(date[0]));
}

void start()
{
    Database db;
    std::string line;
    while (std::getline(std::cin, line))
    {
        try {
            if (std::count(line.begin(), line.end(), ' ') == line.length() || line.empty())
                continue;
            std::stringstream s(line);
            std::string command, date, even;
            s >> command >> date >> even;

            if (command == "Add")
                db.AddEvent(getDate(date), even);
            else if (command == "Del")
            {
                if (even == "")
                {
                    int n = db.DeleteDate(getDate(date));
                    std::cout << "Deleted " << n << " events" << std::endl;
                }
                else
                {
                    if (db.DeleteEvent(getDate(date), even))
                        std::cout << "Deleted successfully" << std::endl;
                    else
                        std::cout << "Event not found" << std::endl;
                }
            }
            else if (command == "Find")
            {
                for (const auto& e : db.Find(getDate(date)))
                    std::cout << e << std::endl;
            }
            else if (command == "Print")
                db.Print();
            else
                std::cout << ("Unknown command: " + command) << std::endl;
        }
        catch (std::runtime_error& re)
        {
            std::cout << re.what() << std::endl;
        }
    }
}

int main()
{
    start();
    return 0;
}
