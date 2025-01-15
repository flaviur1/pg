#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        //TODO
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO

        return glm::lookAt(cameraPosition, cameraTarget, this->cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        // Calculează direcția front din cameraTarget
        glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPosition);

        // Vector pentru mișcarea laterală
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUpDirection));

        switch (direction) {
        case MOVE_FORWARD:
            cameraPosition += cameraFront * speed;
            break;
        case MOVE_BACKWARD:
            cameraPosition -= cameraFront * speed;
            break;
        case MOVE_LEFT:
            cameraPosition -= cameraRight * speed;
            break;
        case MOVE_RIGHT:
            cameraPosition += cameraRight * speed;
            break;
        }

        // Actualizează cameraTarget pentru a rămâne orientată corect
        cameraTarget = cameraPosition + cameraFront;
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        glm::vec3 newCameraTarget = cameraTarget - cameraPosition;
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), cameraUpDirection);
        newCameraTarget = glm::vec3(rotationMatrix * glm::vec4(newCameraTarget, 1.0f));
        newCameraTarget = glm::normalize(newCameraTarget);

        glm::vec3 right = glm::cross(cameraUpDirection, newCameraTarget);
        rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), right);
        newCameraTarget = glm::vec3(rotationMatrix * glm::vec4(newCameraTarget, 1.0f));
        newCameraTarget = glm::normalize(newCameraTarget);

        if (glm::abs(glm::dot(newCameraTarget, cameraUpDirection)) < 0.99)
        {
            cameraTarget = cameraPosition + newCameraTarget;
        }

        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraUpDirection, cameraFrontDirection));
    }
}
