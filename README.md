# How To Work
</br>
You just need to set your desired IP in line 4 and your desired port in line 5. Then in line 14, enter the hour and minute offset of your country instead of the entry of the <strong>SetOffset</strong> function in seconds.
Then call the <strong>GetDateTime()</strong> function anywhere in the code, it will return a string consisting of the date and time at the moment.
If you do not enter offset, you will see UTC Datetime.
</br>
I hope it will be useful for you.
</br>
<strong>If you are from Iran, the time.nist.gov site may not work sometimes; So preferably use an internal IP.</strong>
