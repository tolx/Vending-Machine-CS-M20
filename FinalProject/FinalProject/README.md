# Project Source Folder
This directory is for files and folders that will be directly accessed by the program. Do NOT put junk here.

##Input file documentation (VM001-Command-Input.txt)

Return to idle After making selection (i.e. A5 goes back to idle state)

Cancel? Return to idle state

CoinReturn? return to idle state




====  User Commands ==== 


A Block can have up to 5 user commands

1. Cash (value)

2. Credit (AP|DC)
Ap = Accepted, DC = Declined

3. Selection A1 -> F5 (After selection go to CoinReturn state)

4. Cancel  (Exit point to return to idle state) -> automatically actives coinReturn if coins were inserted. 

5. CoinReturn (Exit point to return to idle state)





Currency


Acceptable input for cash: .01, .05, .1, .25, .5, 1, 5

^NOTE: Leading and trailing zero's will also be accepted, meaning 1.00, 1, or 01 are all okay, but 10 or $1 are not.

^Also NOTE: The "$" symbol will be included in our virtual COUT, so the user would not type it.

Bill $1, $5

Coin Pennie(0.01) , Nickel(0.05), Dime(0.10), Quarter(0.25), HalfDollar(0.50), Dollar(1.00)
