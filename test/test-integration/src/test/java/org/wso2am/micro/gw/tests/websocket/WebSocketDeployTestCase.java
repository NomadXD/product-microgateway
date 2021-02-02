package org.wso2am.micro.gw.tests.websocket;

import com.github.dockerjava.zerodep.shaded.org.apache.hc.core5.http.HttpStatus;
import io.netty.handler.codec.http.HttpHeaderNames;
import org.testng.Assert;
import org.testng.annotations.AfterClass;
import org.testng.annotations.BeforeClass;
import org.testng.annotations.Test;
import org.wso2am.micro.gw.tests.common.BaseTestCase;
import org.wso2am.micro.gw.tests.context.MicroGWTestException;
import org.wso2am.micro.gw.tests.util.ApiProjectGenerator;
import org.wso2am.micro.gw.tests.util.HttpResponse;
import org.wso2am.micro.gw.tests.util.HttpsPostMultipart;
import org.wso2am.micro.gw.tests.util.TestConstant;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class WebSocketDeployTestCase extends BaseTestCase {
    @BeforeClass
    public void startMGW() throws MicroGWTestException, IOException, InterruptedException {
        super.startMGW();
    }

    @Test(description = "Test to check the api deployment is working")
    public void webSocketDeployTest() throws Exception{
        //api yaml file should put to the resources/apis/openApis folder
        String apiZipfile = ApiProjectGenerator.createApictlProjZip(null, null, "apis/openApis/mockWebSocketApiProdSand.yaml");

        // Set header
        Map<String, String> headers = new HashMap<String,String>();
        headers.put(HttpHeaderNames.AUTHORIZATION.toString(), "Basic YWRtaW46YWRtaW4=");
        HttpsPostMultipart multipart = new HttpsPostMultipart(getImportAPIServiceURLHttps(
                TestConstant.ADAPTER_IMPORT_API_RESOURCE) , headers);
        multipart.addFilePart("file", new File(apiZipfile));
        HttpResponse response = multipart.getResponse();

        Assert.assertNotNull(response);
        Assert.assertEquals(response.getResponseCode(), HttpStatus.SC_OK,"Response code mismatched");
    }

    @AfterClass
    public void stopMGW() {
        super.stopMGW();
    }
}
