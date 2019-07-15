passed_num=0
failed_num=0

test_error() {
    if [ $? -eq 1 ] ; then
        echo " (OK - finished with error)"
        passed_num=$((passed_num+1))
    else
        echo " (FAIL - finished successfully) "
        failed_num=$((failed_num+1))
    fi
}

test_valid() {
    if [ $? -eq 0 ] ; then
        echo " (OK - finished successfully) "
        passed_num=$((passed_num+1))
    else
        echo " (FAIL - finished with error) "
        failed_num=$((failed_num+1)) 
    fi
}

echo -e "\n*** Test info ***"
echo -e "You can find input files in tests/input/"
echo -e "You can find manually parsed results in tests/results/"

echo -e "\n*** Running tests ***"

echo -e "\n ** Parameters **"

printf "  - No parameters"
./feedreader > /dev/null 2>&1
test_error

printf "  - Printing help"
./feedreader -h > /dev/null 2>&1
test_valid

printf "  - Passing doubled parameter"
./feedreader -T -T > /dev/null 2>&1
test_error

printf "  - Combining feedfile and URL"
./feedreader -f data/source_list.txt http://url.com > /dev/null 2>&1
test_error

printf "  - Not valid parameter / more URLs"
./feedreader http://url1.com http://url2.com > /dev/null 2>&1
test_error


echo -e "\n ** Connection (downloading document)**"

printf "  - Testing HTTP with default port (http://www.fit.vutbr.cz/news/news-rss.php)"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php > /dev/null 2>&1
test_valid

printf "  - Testing HTTP with custom port (http://www.fit.vutbr.cz:80/news/news-rss.php)"
./feedreader http://www.fit.vutbr.cz:80/news/news-rss.php > /dev/null 2>&1
test_valid

printf "  - Testing HTTP : not existing URL (http://www.fit.vutbr.cz/NOT_VALID)"
./feedreader http://www.fit.vutbr.cz/NOT_VALID > /dev/null 2>&1
test_error

printf "  - Testing HTTPS with default port (https://tools.ietf.org/dailydose/dailydose_atom.xml)"
./feedreader https://tools.ietf.org/dailydose/dailydose_atom.xml > /dev/null 2>&1
test_valid

printf "  - Testing HTTPS with custom port (https://tools.ietf.org:443/dailydose/dailydose_atom.xml)"
./feedreader https://tools.ietf.org:443/dailydose/dailydose_atom.xml > /dev/null 2>&1
test_valid

printf "  - Testing HTTPS : not existing URL (https://tools.ietf.org/dailydose/NOT_VALID)"
./feedreader https://tools.ietf.org/dailydose/NOT_VALID > /dev/null 2>&1
test_error

printf "  - Testing HTTPS with /dev/null as certaddr (https://tools.ietf.org/dailydose/dailydose_atom.xml)"
./feedreader https://tools.ietf.org/dailydose/dailydose_atom.xml -C /dev/null > /dev/null 2>&1
test_error

printf "  - Testing not valid URL (abc.ietf.orga)"
./feedreader abc.ietf.orga > /dev/null 2>&1
test_error


echo -e "\n ** Parsing ATOM ** "

printf "  - Parsing corrupted XML content (tests/input/atom/corrupted.xml)"
cat ./tests/input/atom/corrupted.xml | ./feedreader -p > /dev/null 2>&1
test_error

printf "  - Parsing valid XML content : no -Tua params (tests/input/atom/valid_1.xml)"
cat ./tests/input/atom/valid_1.xml | ./feedreader -p | diff ./tests/results/atom/valid_1_no_params.result -
test_valid

printf "  - Parsing valid XML content : Author only (tests/input/atom/valid_1.xml)"
cat ./tests/input/atom/valid_1.xml | ./feedreader -p -a | diff ./tests/results/atom/valid_1_a_param.result -
test_valid

printf "  - Parsing valid XML content : Link only (tests/input/atom/valid_1.xml)"
cat ./tests/input/atom/valid_1.xml | ./feedreader -p -u | diff ./tests/results/atom/valid_1_u_param.result -
test_valid

printf "  - Parsing valid XML content : Date only (tests/input/atom/valid_1.xml)"
cat ./tests/input/atom/valid_1.xml | ./feedreader -p -T | diff ./tests/results/atom/valid_1_T_param.result -
test_valid

printf "  - Parsing valid XML content : All params -Tua (tests/input/atom/valid_1.xml)"
cat ./tests/input/atom/valid_1.xml | ./feedreader -p -Tua | diff ./tests/results/atom/valid_1_all_params.result -
test_valid


echo -e "\n ** Parsing RSSv1 ** "

printf "  - Parsing corrupted XML content (tests/input/rss_v1/corrupted.xml)"
cat ./tests/input/rss_v1/corrupted.xml | ./feedreader -p > /dev/null 2>&1
test_error

printf "  - Parsing valid XML content : no -Tua params (tests/input/rss_v1/valid_1.xml)"
cat ./tests/input/rss_v1/valid_1.xml | ./feedreader -p | diff ./tests/results/rss_v1/valid_1_no_params.result -
test_valid

printf "  - Parsing valid XML content : Author only (tests/input/rss_v1/valid_1.xml)"
cat ./tests/input/rss_v1/valid_1.xml | ./feedreader -p -a | diff ./tests/results/rss_v1/valid_1_a_param.result -
test_valid

printf "  - Parsing valid XML content : Link only (tests/input/rss_v1/valid_1.xml)"
cat ./tests/input/rss_v1/valid_1.xml | ./feedreader -p -u | diff ./tests/results/rss_v1/valid_1_u_param.result -
test_valid

printf "  - Parsing valid XML content : Date only (tests/input/rss_v1/valid_1.xml)"
cat ./tests/input/rss_v1/valid_1.xml | ./feedreader -p -T | diff ./tests/results/rss_v1/valid_1_T_param.result -
test_valid

printf "  - Parsing valid XML content : All params -Tua (tests/input/rss_v1/valid_1.xml)"
cat ./tests/input/rss_v1/valid_1.xml | ./feedreader -p -Tua | diff ./tests/results/rss_v1/valid_1_all_params.result -
test_valid


echo -e "\n ** Parsing RSSv2 ** "

printf "  - Parsing corrupted XML content (tests/input/rss_v2/corrupted.xml)"
cat ./tests/input/rss_v2/corrupted.xml | ./feedreader -p > /dev/null 2>&1
test_error

printf "  - Parsing valid XML content : no -Tua params (tests/input/rss_v2/valid_1.xml)"
cat ./tests/input/rss_v2/valid_1.xml | ./feedreader -p | diff ./tests/results/rss_v2/valid_1_no_params.result -
test_valid

printf "  - Parsing valid XML content : Author only (tests/input/rss_v2/valid_1.xml)"
cat ./tests/input/rss_v2/valid_1.xml | ./feedreader -p -a | diff ./tests/results/rss_v2/valid_1_a_param.result -
test_valid

printf "  - Parsing valid XML content : Link only (tests/input/rss_v2/valid_1.xml)"
cat ./tests/input/rss_v2/valid_1.xml | ./feedreader -p -u | diff ./tests/results/rss_v2/valid_1_u_param.result -
test_valid

printf "  - Parsing valid XML content : Date only (tests/input/rss_v2/valid_1.xml)"
cat ./tests/input/rss_v2/valid_1.xml | ./feedreader -p -T | diff ./tests/results/rss_v2/valid_1_T_param.result -
test_valid

printf "  - Parsing valid XML content : All params -Tua (tests/input/rss_v2/valid_1.xml)"
cat ./tests/input/rss_v2/valid_1.xml | ./feedreader -p -Tua | diff ./tests/results/rss_v2/valid_1_all_params.result -
test_valid

echo -e "\n*** Tests finished ***"
echo "Passed: $passed_num"
echo "Failed: $failed_num"
