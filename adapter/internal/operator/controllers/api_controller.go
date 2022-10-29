/*
Copyright 2022.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package controllers

import (
	"context"
	"github.com/wso2/product-microgateway/adapter/internal/loggers"
	"github.com/wso2/product-microgateway/adapter/internal/operator/synchronizer"
	"sigs.k8s.io/controller-runtime/pkg/controller"
	"sigs.k8s.io/controller-runtime/pkg/handler"
	"sigs.k8s.io/controller-runtime/pkg/manager"
	"sigs.k8s.io/controller-runtime/pkg/source"
	gwapiv1b1 "sigs.k8s.io/gateway-api/apis/v1beta1"

	ctrl "sigs.k8s.io/controller-runtime"
	"sigs.k8s.io/controller-runtime/pkg/client"
	"sigs.k8s.io/controller-runtime/pkg/log"

	dpv1alpha1 "github.com/wso2/product-microgateway/adapter/internal/operator/api/v1alpha1"
)

// APIReconciler reconciles a API object
type APIReconciler struct {
	client client.Client
	//Scheme *runtime.Scheme
	ods *synchronizer.OperatorDataStore
	ors *synchronizer.OperatorReferenceStore
}

func NewAPIController(mgr manager.Manager, operatorDataStore *synchronizer.OperatorDataStore, operatorReferenceStore *synchronizer.OperatorReferenceStore) error {
	r := &APIReconciler{
		client: mgr.GetClient(),
		ods:    operatorDataStore,
		ors:    operatorReferenceStore,
	}
	c, err := controller.New("API", mgr, controller.Options{Reconciler: r})
	if err != nil {
		loggers.LoggerOperator.Errorf("Error creating API controller: %v", err)
		return err
	}

	if err := c.Watch(&source.Kind{Type: &dpv1alpha1.API{}}, &handler.EnqueueRequestForObject{}); err != nil {
		loggers.LoggerOperator.Errorf("Error watching API resources: %v", err)
		return err
	}
	loggers.LoggerOperator.Info("API Controller successfully started. Watching API Objects....")
	return nil
}

//+kubebuilder:rbac:groups=dp.wso2.com,resources=apis,verbs=get;list;watch;create;update;patch;delete
//+kubebuilder:rbac:groups=dp.wso2.com,resources=apis/status,verbs=get;update;patch
//+kubebuilder:rbac:groups=dp.wso2.com,resources=apis/finalizers,verbs=update

// Reconcile is part of the main kubernetes reconciliation loop which aims to
// move the current state of the cluster closer to the desired state.
// TODO(user): Modify the Reconcile function to compare the state specified by
// the API object against the actual cluster state, and then
// perform operations to make the cluster state reflect the state specified by
// the user.
//
// For more details, check Reconcile and its Result here:
// - https://pkg.go.dev/sigs.k8s.io/controller-runtime@v0.13.0/pkg/reconcile
func (r *APIReconciler) Reconcile(ctx context.Context, req ctrl.Request) (ctrl.Result, error) {
	_ = log.FromContext(ctx)
	var apiDef dpv1alpha1.API
	if err := r.client.Get(ctx, req.NamespacedName, &apiDef); err != nil {
		loggers.LoggerOperator.Errorf("apiDef related to reconcile with key: %v not found", req.NamespacedName.String())
	}
	loggers.LoggerOperator.Infof("Reconciled API: %v", apiDef.Spec.APIDisplayName)
	if err := r.ods.AddNewAPI(apiDef, gwapiv1b1.HTTPRoute{}, gwapiv1b1.HTTPRoute{}); err != nil {
		loggers.LoggerOperator.Errorf("Error storing new API in operator data store: %v", err)
	}
	return ctrl.Result{}, nil
}

// SetupWithManager sets up the controller with the Manager.
func (r *APIReconciler) SetupWithManager(mgr ctrl.Manager) error {
	return ctrl.NewControllerManagedBy(mgr).
		For(&dpv1alpha1.API{}).
		Complete(r)
}
