#define lc_TEMPLATES

#include <ESP8266WiFi.h>
#include "templates.h"

const String sHtmlHttpHeader = "\
HTTP/1.1 200 OK\r\n\
Content-Length: <!--LENGTH-->\r\n\
Content-Type: <!--TYPE-->\r\n\
Connection: close\r\n\
\r\n";

const String sHtmlMenu = "\
<html>\
  <head>\
    <title>ESP8266</title>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  </head>\
  <body>\
    <div class=\"container\">\
      <div class=\"menu\">\
         <a href=\"/Sensor/\">Sensor</a>\
         <a href=\"/Network/\">Network</a>\
      </div>";

const String sHtmlSubMenuNetwork = "\
      <div class=\"submenu\">\
        <a href=\"/Network/WiFi/\">WiFi</a>\
        <a href=\"/Network/Accesspoint/\">Accesspoint</a>\
      </div>";

const String sHtmlEnd = "\
    </div>\
  <link rel=\"stylesheet\" href=\"/css/skeleton.css\">\
  </body>\
</html>";

const String sHtml404 = "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";

/*
* Skeleton V2.0.4
* Copyright 2014, Dave Gamache
* www.getskeleton.com
* Free to use under the MIT license.
* http://www.opensource.org/licenses/mit-license.php
* 12/29/2014
*/
const String CSS = ".column,.columns,.container,.u-full-width{width:100%;box-sizing:border-box}h1,h2,h3{letter-spacing:-.1rem}body,h6{line-height:1.6}.container{position:relative;max-width:960px;margin:0 auto;padding:0 20px}ol,p,ul{margin-top:0}.column,.columns{float:left}@media (min-width:400px){.container{width:85%;padding:0}}html{font-size:62.5%}body{font-size:1.5em;font-weight:400;font-family:Raleway,HelveticaNeue,\"Helvetica Neue\",Helvetica,Arial,sans-serif;color:#222}h1,h2,h3,h4,h5,h6{margin-top:0;margin-bottom:2rem;font-weight:300}h1{font-size:4rem;line-height:1.2}h2{font-size:3.6rem;line-height:1.25}h3{font-size:3rem;line-height:1.3}h4{font-size:2.4rem;line-height:1.35;letter-spacing:-.08rem}h5{font-size:1.8rem;line-height:1.5;letter-spacing:-.05rem}h6{font-size:1.5rem;letter-spacing:0}@media (min-width:550px){.container{width:80%}.column,.columns{margin-left:4%}.column:first-child,.columns:first-child{margin-left:0}.one.column,.one.columns{width:4.66666666667%}.two.columns{width:13.3333333333%}.three.columns{width:22%}.four.columns{width:30.6666666667%}.five.columns{width:39.3333333333%}.six.columns{width:48%}.seven.columns{width:56.6666666667%}.eight.columns{width:65.3333333333%}.nine.columns{width:74%}.ten.columns{width:82.6666666667%}.eleven.columns{width:91.3333333333%}.twelve.columns{width:100%;margin-left:0}.one-third.column{width:30.6666666667%}.two-thirds.column{width:65.3333333333%}.one-half.column{width:48%}.offset-by-one.column,.offset-by-one.columns{margin-left:8.66666666667%}.offset-by-two.column,.offset-by-two.columns{margin-left:17.3333333333%}.offset-by-three.column,.offset-by-three.columns{margin-left:26%}.offset-by-four.column,.offset-by-four.columns{margin-left:34.6666666667%}.offset-by-five.column,.offset-by-five.columns{margin-left:43.3333333333%}.offset-by-six.column,.offset-by-six.columns{margin-left:52%}.offset-by-seven.column,.offset-by-seven.columns{margin-left:60.6666666667%}.offset-by-eight.column,.offset-by-eight.columns{margin-left:69.3333333333%}.offset-by-nine.column,.offset-by-nine.columns{margin-left:78%}.offset-by-ten.column,.offset-by-ten.columns{margin-left:86.6666666667%}.offset-by-eleven.column,.offset-by-eleven.columns{margin-left:95.3333333333%}.offset-by-one-third.column,.offset-by-one-third.columns{margin-left:34.6666666667%}.offset-by-two-thirds.column,.offset-by-two-thirds.columns{margin-left:69.3333333333%}.offset-by-one-half.column,.offset-by-one-half.columns{margin-left:52%}h1{font-size:5rem}h2{font-size:4.2rem}h3{font-size:3.6rem}h4{font-size:3rem}h5{font-size:2.4rem}h6{font-size:1.5rem}}a{color:#1EAEDB}a:hover{color:#0FA0CE}.button,button,input[type=submit],input[type=reset],input[type=button]{display:inline-block;height:38px;padding:0 30px;color:#555;text-align:center;font-size:11px;font-weight:600;line-height:38px;letter-spacing:.1rem;text-transform:uppercase;text-decoration:none;white-space:nowrap;background-color:transparent;border-radius:4px;border:1px solid #bbb;cursor:pointer;box-sizing:border-box}.button:focus,.button:hover,button:focus,button:hover,input[type=submit]:focus,input[type=submit]:hover,input[type=reset]:focus,input[type=reset]:hover,input[type=button]:focus,input[type=button]:hover{color:#333;border-color:#888;outline:0}.button.button-primary,button.button-primary,input[type=submit].button-primary,input[type=reset].button-primary,input[type=button].button-primary{color:#FFF;background-color:#33C3F0;border-color:#33C3F0}.button.button-primary:focus,.button.button-primary:hover,button.button-primary:focus,button.button-primary:hover,input[type=submit].button-primary:focus,input[type=submit].button-primary:hover,input[type=reset].button-primary:focus,input[type=reset].button-primary:hover,input[type=button].button-primary:focus,input[type=button].button-primary:hover{color:#FFF;background-color:#1EAEDB;border-color:#1EAEDB}input[type=tel],input[type=url],input[type=password],input[type=email],input[type=number],input[type=search],input[type=text],select,textarea{height:38px;padding:6px 10px;background-color:#fff;border:1px solid #D1D1D1;border-radius:4px;box-shadow:none;box-sizing:border-box}input[type=tel],input[type=url],input[type=password],input[type=email],input[type=number],input[type=search],input[type=text],textarea{-webkit-appearance:none;-moz-appearance:none;appearance:none}textarea{min-height:65px;padding-top:6px;padding-bottom:6px}input[type=tel]:focus,input[type=url]:focus,input[type=password]:focus,input[type=email]:focus,input[type=number]:focus,input[type=search]:focus,input[type=text]:focus,select:focus,textarea:focus{border:1px solid #33C3F0;outline:0}label,legend{display:block;margin-bottom:.5rem;font-weight:600}fieldset{padding:0;border-width:0}input[type=checkbox],input[type=radio]{display:inline}label>.label-body{display:inline-block;margin-left:.5rem;font-weight:400}ul{list-style:circle inside}ol{list-style:decimal inside}ol,ul{padding-left:0}ol ol,ol ul,ul ol,ul ul{margin:1.5rem 0 1.5rem 3rem;font-size:90%}.button,button,li{margin-bottom:1rem}code{padding:.2rem .5rem;margin:0 .2rem;font-size:90%;white-space:nowrap;background:#F1F1F1;border:1px solid #E1E1E1;border-radius:4px}pre>code{display:block;padding:1rem 1.5rem;white-space:pre}td,th{padding:12px 15px;text-align:left;border-bottom:1px solid #E1E1E1}td:first-child,th:first-child{padding-left:0}td:last-child,th:last-child{padding-right:0}fieldset,input,select,textarea{margin-bottom:1.5rem}blockquote,dl,figure,form,ol,p,pre,table,ul{margin-bottom:2.5rem}.u-max-full-width{max-width:100%;box-sizing:border-box}.u-pull-right{float:right}.u-pull-left{float:left}hr{margin-top:3rem;margin-bottom:3.5rem;border-width:0;border-top:1px solid #E1E1E1}.container:after,.row:after,.u-cf{content:\"\";display:table;clear:both}.connected{background-color:#FFD400;}";


const String sHtmlNetworkOverView = "\
<div class=\"container\">\
    <div class=\"row\">\
        <h4>AP</h4>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Status</div>\
        <div class=\"columns nine\"><!--APACTIVE--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Name</div>\
        <div class=\"columns nine\"><!--APNAME--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Password</div>\
        <div class=\"columns nine\"><!--APPW--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">IP</div>\
        <div class=\"columns nine\"><!--APIP--></div>\
    </div>\
    <div class=\"row\">\
        <h4>Wifi</h4>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Status</div>\
        <div class=\"columns nine\"><!--WIFISTATE--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">MAC-Address</div>\
        <div class=\"columns nine\"><!--MAC--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Name</div>\
        <div class=\"columns nine\"><!--WIFINAME--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Password</div>\
        <div class=\"columns nine\"><!--WIFIPW--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">IP</div>\
        <div class=\"columns nine\"><!--WIFIIP--></div>\
    </div>\
</div>";

const String sHtmlNetworkDisconnect = "\
<form mehtod=\"get\" action=\"/Network/connect/\">\
  <div class=\"row\">\
    <div class=\"eight columns\">\
      <label>Wifi</label>\
      <input class=\"u-full-width\" type=\"text\" name=\"ssid\" value=\"<!--SSID-->\" readonly>\
    </div>\
    <div class=\"four columns\">\
      <label></label>\
      <input class=\"u-full-width\" type=\"submit\" value=\"disconnect\">\
    </div>\
  </div>\
  <input type=\"hidden\" name=\"action\" value=\"disconnect\">\
</form>";

const String sHtmlNetworkScan = "\
<form mehtod=\"get\" action=\"/Network/connect/\">\
  <div class=\"row <!--CONNECTED-->\">\
    <div class=\"four columns\">\
      <label>Wifi</label>\
      <input class=\"u-full-width\" type=\"text\" name=\"ssid\" value=\"<!--SSID-->\" readonly>\
    </div>\
    <div class=\"four columns\">\
      <label>Password</label>\
      <input class=\"u-full-width\" type=\"text\" name=\"pass\" placeholder=\"Password or empty\">\
    </div>\
    <div class=\"four columns\">\
      <label>Connect</label>\
      <input class=\"u-full-width\" type=\"submit\" value=\"connect\">\
    </div>\
  </div>\
  <input type=\"hidden\" name=\"action\" value=\"connect\">\
</form>";

const String sHtmlSensorOverView = "\
  <div class=\"container\">\
    <div class=\"row\">\
        <h4>Sensor</h4>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Type</div>\
        <div class=\"columns nine\"><!--TYPE--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">value</div>\
        <div class=\"columns nine\"><!--VALUE--></div>\
    </div>\
    <div class=\"row\">\
        <div class=\"columns three\">Unity</div>\
        <div class=\"columns nine\"><!--UNITY--></div>\
    </div>\
    <form action=\"/Sensor/Set\" mehtod=\"GET\">\
      <div class=\"row\">\
        <div class=\"columns four\">\
          <label>API-KEY</label>\
          <input type=\"text\" class=\"u-full-width\" name=\"key\" value=\"<!--APIKEY-->\">\
        </div>\
        <div class=\"columns four\">\
          <label>Field-Number</label>\
          <input type=\"number\" class=\"u-full-width\" name=\"field\" min=1 max=8 value=\"<!--APIFIELD-->\">\
        </div>\
        <div class=\"columns four\">\
          <label>Intervall (Minutes)</label>\
          <input type=\"number\" class=\"u-full-width\" name=\"intervall\" min=1 max=500 value=\"<!--APIINTERVALL-->\">\
        </div>\
      </div>\
      <div class=\"row\">\
        <input type=\"submit\" value=\"save\">\
      </div>\
    </form>\
    <form action=\"/Deepsleep/\" method=\"get\">\
      <label>The ESP8266 will go to deepsleep after this!</label>\
      <input type=\"submit\" name=\"start\" value=\"start\">\
    </form>\
  </div>\
";
  
const String sHtmlNetworkApConfig = "\
<form action=\"/Network/Accesspoint/\" method=\"GET\">\
  <div class=\"row\">\
    <div class=\"columns four\">\
      <label>Name</label>\
      <input type=\"text\" class=\"u-full-width\" name=\"ssid\"  value=\"<!--APNAME-->\">\
    </div>\
    <div class=\"columns four\">\
      <label>Password</label>\
      <input type=\"text\" class=\"u-full-width\"  name=\"pw\"    value=\"<!--APPW-->\">\
    </div>\
    <div class=\"columns two\">\
      <input type=\"submit\" class=\"u-full-width\" value=\"save\">\
    </div>\
    <input type=\"hidden\" name=\"action\" value=\"configure\">\
  </div>\
</form>\
<form action=\"/Network/Accesspoint/\" method=\"GET\">\
  <div class=\"row\">\
    <input type=\"hidden\" name=\"action\" value=\"ed\">\
    <input type=\"submit\" name=\"state\" value=\"<!--APEA-->able\">\
  </div>\
</form>";
