# How To Work
</br>
You just need to set your desired IP in line 9 and then in line 13, enter the hour and minute offset of your country instead of the entry of the <strong>SetOffset</strong> function in seconds.
Then call the <strong>GetDateTime()</strong> function anywhere in the code, it will return a string consisting of the date and time.
If you do not enter offset, you will see UTC Datetime.