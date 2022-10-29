package synchronizer

import "github.com/wso2/product-microgateway/adapter/internal/operator/api/v1alpha1"
import gwapiv1b1 "sigs.k8s.io/gateway-api/apis/v1beta1"

// +k8s:deepcopy-gen=true
type APIState struct {
	APIDefinition *v1alpha1.API
	ProdHttpRoute *gwapiv1b1.HTTPRoute
	SandHttpRoute *gwapiv1b1.HTTPRoute
}
